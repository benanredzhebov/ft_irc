/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:36:20 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/20 13:12:11 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"


/*Used to listen for incoming connections from clients*/
int Server::_creatingServerSocketFd(){
	_server_fdsocket = socket(AF_INET6, SOCK_STREAM, 0);
	if (_server_fdsocket < 0 ){
		std::cout << "socket creation failed\n" << std::endl;
		return (0);
	}
	return (1);
}

int Server::_serverReservePortandIpBind(){
	int opt = 0;
	struct sockaddr_in6	address;

	memset(&address, 0, sizeof(address));
	address.sin6_family = AF_INET6;
	address.sin6_port = htons(_port);
	address.sin6_addr = in6addr_any;
	if (setsockopt(_server_fdsocket, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) < 0) {
		std::cerr << "Failed to set IPV6_V6ONLY option: " << strerror(errno) << std::endl;
		return (0);
	}
	if (bind(_server_fdsocket, (const struct sockaddr *)&address, sizeof(address)) < 0){
		return (std::cout << "binding failed\n" << std::endl, 0);
	}
	return (1);

}

int Server::_serverListens() {
	if (listen(_server_fdsocket, SOMAXCONN) < 0) {
		std::cout << "listen failed\n" << std::endl;
		return (0);
	}
	return (1);
}

int Server::_createBindListen(){
	if (!_creatingServerSocketFd())
		return (0);
	if (!_serverReservePortandIpBind())
		return (0);
	if (!_serverListens())
		return (0);
	std::cout << GRE << "Server fd [" << _server_fdsocket << "] connected" << RESET << std::endl;
	return (1);
}

int	Server::_serverAcceptIncoming(){
	int					clientFd;
	socklen_t			clientAddrLen;
	struct sockaddr_in6	clientAddr;

	clientAddrLen = sizeof(clientAddr);
	clientFd = accept(_server_fdsocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
	if (clientFd  < 0) {
		std::cerr << "accepting client failed\n" << std::endl;
		close_fds(clientFd);
		return (-1);
	}
	
	if (DEBUG) {
		char ipStr[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &clientAddr.sin6_addr, ipStr, sizeof(ipStr));
		std::cout << "Debug: New connection attempt from " << ipStr << std::endl;
	}

	return (clientFd);
}

int Server::_run_server() {
	int			clientFD;
	epfd = epoll_create1(0);
	if (epfd == -1) {
		std::cerr << "Failed creating an instance of epoll" << std::endl;
		return (0);
	}

	if (!_setServerSocket())
		return (0);
	while (1) {
		epoll_waitfd = epoll_wait(epfd, _fds.data(), _fds.size(), -1);
		if (epoll_waitfd < 0)
			break ;
		if (resizeFds())
			continue;
		for (int i = 0; i < epoll_waitfd; i++) {
			clientFD = _fds[i].data.fd;
			if (clientFD == _server_fdsocket) {
				clientFD = _serverAcceptIncoming();
				if (clientFD < 0){
					continue ;
				}
				client_event = initEpollEvent(EPOLLIN, clientFD);
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, clientFD, &client_event) == -1) {
					close_fds(clientFD);
					std::cerr << "Failed to add client to epoll\n";
					continue;
				}

				// Retrieve the client's address information
				struct sockaddr_in6 cliaddr;
				socklen_t cliaddr_len = sizeof(cliaddr);
				if (getpeername(clientFD, (struct sockaddr*)&cliaddr, &cliaddr_len) == -1) {
					std::cerr << "Failed to get client address\n";
					close_fds(clientFD);
					continue;
				}
				
				Client newclient;
				newclient.setClientFd(clientFD);
				char	ipStr[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, &cliaddr.sin6_addr, ipStr, sizeof(ipStr));
				newclient.setIpAdd(ipStr);
				_clients.push_back(newclient);

				// Debug information
				if (DEBUG)
					std::cout << "Debug: New client connected from " << ipStr << std::endl;
			}
			else {
				Client *cli = getClient(clientFD);
				if (!cli) {
					std::cerr << "Client FD " << clientFD << " not found in _clients. Skipping." << std::endl;
					continue;
				}
				handleClientInput(cli);
			}
		}
	}
	return (1);
}

int	Server::searchForClients(std::string nickname)
{
	int count = 0;
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getClientInChannel(nickname))
			count++;
	}
	return count;
}

/*process message that have been buffered for a client.When a client has been suspended 
and messages sent to the client have been stored in a queue. When the client resumes, the function
deliveres those buffered messages*/
void Server::processBufferedMessages(Client* client) {
	while (!client->isMessageQueueEmpty()) {
		std::string message = client->getNextMessageFromQueue();
		std::vector<std::string> splited_cmd = split(message, ' ');
		handleClientInput(client);
	}
}
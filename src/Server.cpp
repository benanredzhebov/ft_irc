/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:36:20 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/30 20:29:39 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int Server::_creatingServerSocketFd(){
	_server_fdsocket = socket(AF_INET6, SOCK_STREAM, 0);
	if (_server_fdsocket < 0 ){
		std::cout << "socket creation faliled\n" << std::endl;
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
		return (std::cout << "binding faliled\n" << std::endl, 0);
	}
	return (1);

}

int Server::_serverListens() {
	if (listen(_server_fdsocket, SOMAXCONN) < 0) {
		std::cout << "lsiten faliled\n" << std::endl;
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
	std::cout << "\ncreating, binding and listening successfully\n" << std::endl;
	return (1);
}

int	Server::_serverAcceptIncoming(){
	int					clientFd;
	socklen_t			clientAddrLen;
	struct sockaddr_in6	clientAddr;

	clientAddrLen = sizeof(clientAddr);
	clientFd = accept(_server_fdsocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
	if (clientFd  < 0) {
		std::cerr << "accepting client faliled\n" << std::endl;
		close_fds(clientFd);
		return (-1);
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
			if (_fds[i].data.fd == _server_fdsocket) {
				clientFD = _serverAcceptIncoming();
				if (clientFD < 0){
					continue ;
				}
				client_event = initEpollEvant(EPOLLIN, clientFD);
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, clientFD, &client_event) == -1) {
					close_fds(clientFD);
            		std::cerr << "Failed to add client to epoll\n";
            		continue;
				}
				Client newclient;
				newclient.setClientFd(clientFD);
				_clients.push_back(newclient);
			}
			else {
				clientFD = _fds[i].data.fd;
				Client *cli = getClient(clientFD);
				handleClientInput(cli);
			}
		}
	}
	close_fds(_server_fdsocket);
	return (1);
}

int		Server::searchForClients(std::string nickname) {
	Client *cli;

	cli = getClientNick(nickname);
	return (cli->getChannelSize());
}

// int Server::searchForClients(std::string nickname)
// {
// 	int count = 0;
// 	for (size_t i = 0; i < this->_channels.size(); i++)
// 	{
// 		if (this->_channels[i].getClientInChannel(nickname))
// 			count++;
// 	}
// 	return count;
// }
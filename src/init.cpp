/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:09:32 by danevans          #+#    #+#             */
/*   Updated: 2024/12/23 23:33:50 by danevans         ###   ########.fr       */
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

int Server::_runServerUtils(int x, int fd) {
	if (x == 0){
		std::cerr << "\fd already existfailed\n" << std::endl;
		return (1) ;
	}
	else if (x == -1){
		removeClient(fd);
		return (1) ;
	}
	return (0);
}

//currently we can connect with all clients and then authenticate. user and nick.
// next is to breakdown what ever clients send and then we can use that to know what next to do
// creating channel, joining channel, prvmsg, 
//also need to hook the signal 
int Server::_run_server() {
	epfd = epoll_create1(0);
	Client		client;

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
				client.setClientFd(_serverAcceptIncoming());
				if (client.getFd() < 0){
					return (close_fds(client.getFd()), 0);
				}
				if (_runServerUtils(client_authen(client.getFd()), client.getFd()))
					continue ;
				client.setRegistered(true);
				_clients.push_back(client);
				clientInfoSave(client.getFd());
				client_event = initEpollEvant(EPOLLIN,client.getFd());
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, client.getFd(), &client_event) == -1)
					return (0);
			}
		}
	}
	close_fds(_server_fdsocket);
	return (1);
}
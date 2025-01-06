/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_orthodox.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:45:20 by danevans          #+#    #+#             */
/*   Updated: 2025/01/06 10:10:10 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(int port, const std::string& password) 
	:	_port(port),
		_server_fdsocket(-1),
		_password(password),
		epfd(-1),
		_fds(10),
		epoll_waitfd(0) {
}

Server::~Server() {
	for (int i = 0; i < _clients.size(); i++) {
		removeClientfromChannel(&_clients[i]);
	}
	std::stringstream ss;
	ss << RED << "SERVER [" << _server_fdsocket << "] Terminated 👋\n" << RESET;
	std::string message = ss.str();
	sendAllClient(message);
	if (_server_fdsocket >= 0) {
		std::cout << "\n" << message << std::endl; 
	    close_fds(_server_fdsocket);
	}
	if (epfd >= 0) {
		close_fds(epfd);
	}
}

Server::Server(const Server &other)
	:	_port(other._port),
		_server_fdsocket(other._server_fdsocket),
		_password(other._password),
		_fds(other._fds),
		epfd(other.epfd),
		epoll_waitfd(other.epoll_waitfd) {
}

Server &Server::operator=(const Server &other) {
    if (this != &other) {
        if (_server_fdsocket >= 0) {
            close(_server_fdsocket);
        }
        if (epfd >= 0) {
            close(epfd);
        }
		_port = other._port;
		_server_fdsocket = other._server_fdsocket;
		_password = other._password;
		_fds = other._fds;
		epfd = other.epfd;
		epoll_waitfd = other.epoll_waitfd;
    }
	return *this;
}

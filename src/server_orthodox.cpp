/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_orthodox.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:45:20 by danevans          #+#    #+#             */
/*   Updated: 2025/01/06 00:26:36 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(int port, const std::string& password) 
	:	_port(port),
		_server_fdsocket(-1),
		_password(password),
		epfd(-1),
		_fds(10),
		// _channels(10),
		epoll_waitfd(0) {
		std::cout << "Server paramterized constructed." << std::endl;
}

Server::~Server() {
	// should probably remove all client fd from here , probably not neccasary tho , 
	std::stringstream ss;
	ss << RED << "SERVER [" << _server_fdsocket << "] Terminated\n" << RESET;
	std::string message = ss.str();
	sendAllClient(message);
	if (_server_fdsocket >= 0) {
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
		// _clients(other._clients),
		// _channels(other._channels),
		_fds(other._fds),
		epfd(other.epfd),
		epoll_waitfd(other.epoll_waitfd) {
		std::cout << "Server object copied." << std::endl;
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
		// _clients = other._clients;
		// _channels = other._channels;
		_fds = other._fds;
		epfd = other.epfd;
		epoll_waitfd = other.epoll_waitfd;
		std::cout << "Server object assigned." << std::endl;
    }
	return *this;
}

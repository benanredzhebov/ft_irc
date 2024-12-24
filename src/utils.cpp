/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:26:59 by danevans          #+#    #+#             */
/*   Updated: 2024/12/23 23:17:17 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void	Server::close_fds(int fd){
	if (fd >= 0)
		close(fd);
}

epoll_event	Server::initEpollEvant(int poll_mode, int fd) {
	epoll_event	events;
	events.data.fd = fd;
	events.events = poll_mode;
	return (events);
}

void	Server::removeClient(int fd){
	sendResponse(PASSWORD_AUTH_FAILED, fd);
	close_fds(fd);
}

int	Server::_setServerSocket() {
	if (!_createBindListen())
		return (close_fds(_server_fdsocket), 0);
	server_event = initEpollEvant(EPOLLIN, _server_fdsocket);
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, _server_fdsocket, &server_event) == -1)
		return (0);
	return (1);
}

int	Server::resizeFds() {
	if (epoll_waitfd == _fds.size()){
		_fds.resize(_fds.size() * 2);
		std::cout << "Resized server_fds to: " << _fds.size() << std::endl;	
		return (1);
	}
	return (0);
}
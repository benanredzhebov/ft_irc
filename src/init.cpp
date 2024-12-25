/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 08:49:15 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/25 08:49:25 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

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
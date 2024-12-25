/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:26:59 by danevans          #+#    #+#             */
/*   Updated: 2024/12/25 13:56:24 by danevans         ###   ########.fr       */
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

void	Server::removeClient(int fd) {
	sendResponse(PASSWORD_AUTH_FAILED, fd);
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
    	if (it->getFd() == fd) {
    	    _clients.erase(it);
    	    break;
    	}
	}
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

void Server::handleClientInput(Client* client) {
    switch (client->getState()) {
        case WAITING_FOR_PASSWORD: {
			if (client_authen) {
				client->setState(WAITING_FOR_NICKNAME);
			} else {
				client->decrementPasswordTrials();
				if (client->getPasswordTrials() <= 0) {
				    epoll_ctl(epfd, EPOLL_CTL_DEL, client->getFd(), 0);
				   	removeClient(client->getFd());
				} else {
				    sendResponse("Incorrect password. Try again:", client->getFd());
				}
			}
            break;
        }
        case WAITING_FOR_NICKNAME: {
			if (clientNickName(client->getFd())) {
				client->setState(WAITING_FOR_USERNAME);
			} else {
				sendResponse("Invalid nickname. Try again:", client->getFd());
			}
			break;
        }

        case WAITING_FOR_USERNAME: {
            if (clientUserName(client->getFd())) {
                // client->setState(AUTHENTICATED);
                client->setRegistered(true);
				confirmClientInfo(client->getFd());
            } else {
                sendResponse("Invalid username. Try again:", client->getFd());
            }
            break;
        }

        case AUTHENTICATED: {
            processClientCommand(client, buffer);
            break;
        }
    }
}

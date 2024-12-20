/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:36:20 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/20 14:36:54 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*GETTERS*/

/*Consider a chat server where each connected client is represented by a Client object.
The server needs to find the Client object associated with a specific file descriptor to
send messages or perform other operations.*/
Client *Server::getClient(int fd) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getFd() == fd)
			return &this->_clients[i];
	}
}

/*sends a response (after receiving a request) to a client over a network socket*/
void Server::sendResponse(std::string response, int fd) {
	if (send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;
}

/*consider a chat application where users can join different channels.
The server needs to find a specific channel by its name to perform
operations like sending messages or adding users.*/
Channel *Server::getChannel(std::string name) {
	for (size_t i = 0; i < this->_channels.size(); i++) {
		if (this->_channels[i].getName() == name)
			return &_channels[i];
	}
	return NULL;
}
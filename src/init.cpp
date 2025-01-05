/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:09:32 by danevans          #+#    #+#             */
/*   Updated: 2025/01/05 23:40:18 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Client	*Server::getClient(int fd) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getFd() == fd){
			return &this->_clients[i];
		}
	}
	return (NULL);
}

Client	*Server::getClientNick(std::string nickname) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getNickName() == nickname)
			return &this->_clients[i];
	}
	return (NULL);	
}

void Server::sendResponse(std::string response, int fd) {
	if (send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;
}

bool	Server::is_validNickname(std::string &nickname) {
	if (!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for (size_t i = 1; i < nickname.size(); i++) {
		if (!std::isalnum(nickname[i]) && nickname[i] != '_')
			return false;
	}
	return true;
}

bool	Server::is_validUserName(std::string &username) {
	if (!username.empty() && (username[0] == '&' || username[0] == '#' || username[0] == ':'))
		return false;
	for (size_t i = 1; i < username.size(); i++) {
		if (!std::isalnum(username[i]) && username[i] != '_')
			return false;
	}
	return true;
}

bool	Server::nickNameInUse(std::string &nickname) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getNickName() == nickname)
			return true;
	}
	return false;
}

bool	Server::userNameInUse(std::string &username) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getUserName() == username)
			return true;
	}
	return false;
}

bool Server::_signal = false;

void Server::signalHandler(int signum)
{
	(void)signum;
	Server::_signal = true;
}
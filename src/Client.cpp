/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:17:13 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/24 09:24:26 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client() {
	this->_nickname = "";
	this->_username = "";
	this->_fd = -1;
	this->_isOperator = false;
	this->_registered = false;
	this->_logedin = false;
	this->_buffer = "";
	
	
}
Client::Client(std::string nickname, std::string username, int fd) : _nickname(nickname), _username(username), _fd(fd) {}
Client::Client(const Client &other) {*this = other;}
Client::~Client() {}
Client &Client::operator=(const Client &other) {
	if (this != &other) {
		this->_nickname = other._nickname;
		this->_username = other._username;
		this->_fd = other._fd;
		this->_channelIsInvite = other._channelIsInvite;
		this->_registered = other._registered;
		this->_logedin = other._logedin;
		this->_buffer = other._buffer;
	}
	return *this;
}

/*GETTERS*/

int			Client::getFd() {return this->_fd;}
bool		Client::getRegistered() {return this->_registered;}
bool		Client::getLogedIn() {return this->_logedin;}
std::string Client::getNickName() {return this->_nickname;}
std::string	Client::getUserName() {return this->_username;}

bool		Client::getInviteChannel(std::string &chName) {
	for (size_t i = 0; i < this->_channelIsInvite.size(); i++) {
		if (this->_channelIsInvite[i] == chName)
			return true;
	}
	return false;
}

std::string	Client::getBuffer() {return _buffer;}
std::string	Client::getHostname() {
	std::string hostname = this->getNickName() + "!" + this->getUserName();
	return hostname;
}

/*SETTERS*/

void		Client::setFd(int fd) {this->_fd = fd;}
void		Client::setRegistered(bool value) {this->_registered = value;}
void		Client::setNickName(std::string &nickname) {this->_nickname = nickname;}
void		Client::setUsername(std::string &username) {this->_username = username;}
void		Client::setLogedIn(bool value) {this->_logedin = value;}
void		Client::setBuffer(std::string received) {this->_buffer += received;}

/*METHODS*/

void	Client::clearBuffer() {_buffer.clear();}
void	Client::addChannelInvite(std::string &chname) {
	_channelIsInvite.push_back(chname);
}

void	Client::rmChannelInvite(std::string &chname) {
	for (size_t i = 0; i < this->_channelIsInvite.size(); i++) {
		if (this->_channelIsInvite[i] == chname)
			{this->_channelIsInvite.erase(this->_channelIsInvite.begin() + i); return;}
	}
}
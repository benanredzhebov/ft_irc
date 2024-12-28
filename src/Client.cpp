/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:17:13 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/27 23:00:37 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
    : _fd(-1), 
      _registered(false), 
      _nickbool(false), 
      _logedin(false), 
      _passwordTrials(3),
      _isOperator(false), 
      _nickname(""), 
      _username(""), 
      _buffer(""), 
      _ipadd("") {
	std::cout << "default constructor called\n";
}

Client::Client(const Client& other) 
    :	_fd(other._fd),
		_passwordTrials(other._passwordTrials),
		_registered(other._registered),
		_logedin(other._logedin),
		_isOperator(other._isOperator),
		_passwordVerified(other._passwordVerified),
		_username(other._username),
		_nickname(other._nickname),
		_buffer(other._buffer),
		_ipadd(other._ipadd),
		_nickbool(other._nickbool),
		_channels(other._channels),
      _channelIsInvite(other._channelIsInvite) {
	std::cout << "copy constructor called\n";
	
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        this->_fd = other._fd;
        this->_passwordTrials = other._passwordTrials;
        this->_registered = other._registered;
        this->_logedin = other._logedin;
        this->_isOperator = other._isOperator;
        this->_passwordVerified = other._passwordVerified;
        this->_username = other._username;
        this->_nickname = other._nickname;
        this->_buffer = other._buffer;
        this->_ipadd = other._ipadd;
        this->_nickbool = other._nickbool;
        this->_channels = other._channels;
        this->_channelIsInvite = other._channelIsInvite;
    }
	std::cout << "copy assignment constructor called\n";
    return *this;
}

Client::~Client(){
		std::cout << "destructor calleed" << std::endl;
}

/*GETTERS*/

int				Client::getFd() { return this->_fd; }
bool			Client::getRegistered() { return this->_registered; }
bool			Client::getLogedIn() { return this->_logedin; }
std::string		Client::getNickName() { return this->_nickname; }
std::string		Client::getUserName() { return this->_username; }
std::string		Client::getBuffer() { return _buffer; }
std::string		Client::getIpAdd() { return _ipadd; }

std::string		Client::getHostname() {
    std::string hostname = this->getNickName() + "!" + this->getUserName();
    return hostname;
}

int				Client::getChannelSize() const { return _channels.size(); }

bool			Client::getInviteChannel(std::string &chName) {
    for (size_t i = 0; i < this->_channelIsInvite.size(); i++) {
        if (this->_channelIsInvite[i] == chName)
            return true;
    }
    return false;
}

bool		Client::nickGet() {return this->_nickbool;}
void		Client::decrementPasswordTrials() { _passwordTrials--; }
int 		Client::getPasswordTrials() const { return _passwordTrials; }


/*SETTERS*/
void		Client::setFd(int fd) { this->_fd = fd; }
void		Client::setRegistered(bool value) { this->_registered = value; }
void		Client::setNickName(std::string &nickname) { this->_nickname = nickname; }
void		Client::setUsername(std::string &username) { this->_username = username; }
void		Client::setBuffer(std::string received) { _buffer += received; }
void		Client::setLogedIn(bool value) { this->_logedin = value; }
void		Client::setIpAdd(std::string ipadd) { this->_ipadd = ipadd; }
void		Client::setClientFd(int fd) { _fd = fd; }
void		Client::nickSet(bool value) { _nickbool = value; }

void		Client::rmChannelInvite(std::string &chname){
	std::cout << "remove channel name was called\n";
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:04:19 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/30 21:13:23 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Channel.hpp"

Channel::Channel() {
	this->_invit_only = 0;
	this->_topic = 0;
	this->_key = 0;
	this->_limit = 0;
	this->_topic_restriction = false;
	this->_name = "";
	this->_created_at = "";
	
	// Initialize the _modes vector
	this->_modes.push_back(std::make_pair('i', false));
	this->_modes.push_back(std::make_pair('t', false));
	this->_modes.push_back(std::make_pair('k', false));
	this->_modes.push_back(std::make_pair('o', false));
	this->_modes.push_back(std::make_pair('l', false));


}

Channel::Channel(const Channel &other) {*this = other;}

Channel::~Channel() {}

Channel &Channel::operator=(const Channel &other) {
	if (this != &other) {
		this->_invit_only = other._invit_only;
		this->_topic = other._topic;
		this->_key = other._key;
		this->_limit = other._limit;
		this->_topic_restriction = other._topic_restriction;
		this->_name = other._name;
		this->_time_creation = other._time_creation;
		this->_password = other._password;
		this->_created_at = other._created_at;
		this->_topic_name = other._topic_name;
		this->_clients = other._clients;
		this->_admins = other._admins;
		this->_modes = other._modes;
	}
	return *this;
}

/*SETTERS*/
void	Channel::setInvitOnly(int invit_only) {this->_invit_only = invit_only;}
void	Channel::setTopic(int topic) {this->_topic = topic;}
void	Channel::setKey(int key) {this->_key = key;}
void	Channel::setLimit(int limit) {this->_limit = limit;}
void	Channel::setTopicName(std::string topic_name) {this->_topic_name = topic_name;}
void	Channel::setPassword(std::string password) {this->_password = password;}
void	Channel::setName(std::string name) {this->_name = name;}
void	Channel::setTime(std::string time) {this->_time_creation = time;}
void	Channel::set_topicRestriction(bool value) {this->_topic_restriction = value;}
void	Channel::setModeAtindex(size_t index, bool mode) {_modes[index].second = mode;}
// Sets the creation time of the channel by converting the current time to a string and storing it in the _created_at


void	Channel::set_createiontime() {
	std::time_t	_time = std::time(NULL); // returns the current calendar time
	std::ostringstream	oss;
	oss << _time; // converting current time into the string
	this->_created_at = std::string(oss.str());
}

/*GETTERS*/
int			Channel::getInvitOnly() {return this->_invit_only;}
int			Channel::getTopic() {return this->_topic;}
int			Channel::getKey() {return this->_key;}
int			Channel::getLimit() {return this->_limit;}
int			Channel::getClientsNumber() {return this->_clients.size() + this->_admins.size();}
bool		Channel::gettopic_restriction() const {return this->_topic_restriction;}
bool		Channel::getModeAtindex(size_t index) {return _modes[index].second;}


// It checks if a client with a given nickname is present in the channel
bool		Channel::clientInChannel(std::string &nick) {
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getNickName() == nick)
			return true;
	}
	for (size_t i = 0; i < _admins.size(); i++) {
		if (_admins[i].getNickName() == nick)
			return true;
	}
	return false;
}

std::string	Channel::getTopicName() {return this->_topic_name;}
std::string	Channel::GetPassword() {return this->_password;}
std::string	Channel::getName() {return this->_name;}
std::string	Channel::getTime() {return this->_time_creation;}
std::string	Channel::get_creationtime() {return this->_created_at;}

/*Consider a chat application where channels can have various modes
(e.g., invite-only, topic-restricted). The getModes function constructs
a string representing the active modes of a channel, which can be used
to display the channel's status.
The '+' character serves as a prefix to the list of active modes*/
std::string	Channel::getModes() {
	std::string	mode;
	for (size_t i = 0; i < _modes.size(); i++) {
		if (_modes[i].first != 'o' && _modes[i].second) //For each mode, it checks if the mode identifier is not 'o' and if the mode is active (_modes[i].second is true).
			mode.push_back(_modes[i].first);
	}
	if (!mode.empty())
		mode.insert(mode.begin(), '+');
	return mode;
}

/*Consider a chat application where channels have both regular clients and
admins. The clientChannel_list function constructs a string representing the
list of clients in the channel, with admins prefixed by an '@' symbol*/
std::string	Channel::clientChannel_list() {
	std::string	list;
	for (size_t i = 0; i < _admins.size(); i++) {
		list += "@" + _admins[i].getNickName();
		if ((i + 1) < _admins.size()) //If the current admin is not the last one in the list, it adds a space after the nickname.
			list += " ";
	}
	if (_clients.size())
		list += " ";
		for (size_t i = 0; i < _clients.size(); i++) {
			list += _clients[i].getNickName();
			if ((i + 1) < _clients.size())
				list += " ";
		}
		return list;
}

Client		*Channel::get_client(int fd) {
	for (std::vector<Client>::iterator	it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client		*Channel::get_admin(int fd) {
	for (std::vector<Client>::iterator	it = _admins.begin(); it != _admins.end(); ++it) {
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

/*Consider a chat application where users can join different channels. The server
needs to find a specific client in a channel by their nickname to perform operations
like sending private messages or kicking them out of the channel.*/
Client* Channel::getClientInChannel(std::string name) {
	for (std::vector<Client>::iterator	it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNickName() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it){
		if (it->getNickName() == name)
			return &(*it);
	}
	return NULL;
}

/*METHODS*/

void	Channel::add_client(Client newClient) {_clients.push_back(newClient);}

void	Channel::add_admin(Client newClient) {
	_admins.push_back(newClient);
	// add_client(newClient);
	}

void	Channel::remove_client(int fd) {
	for (std::vector<Client>::iterator	it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getFd() == fd)
			{_clients.erase(it); break;}
	}
}

void	Channel::remove_admin(int fd) {
	for (std::vector<Client>::iterator	it = _admins.begin(); it != _admins.end(); ++it) {
		if (it->getFd() == fd)
			{_admins.erase(it); break;}
	}
}

bool	Channel::change_clientToAdmin(std::string& nick) {
	size_t i = 0;
	for (; i < _clients.size(); i++) {
		if (_clients[i].getNickName() == nick)
			break;
	}
	if (i < _clients.size()) {
		_admins.push_back(_clients[i]);
		_clients.erase(i + _clients.begin());
		return true;
	}
	return false;
}

bool	Channel::change_adminToClient(std::string& nick) {
	size_t i = 0;
	for(; i < _admins.size(); i++){
		if(_admins[i].getNickName() == nick)
			break;
	}
	if(i < _admins.size()){
		_clients.push_back(_admins[i]);
		_admins.erase(i + _admins.begin());
		return true;
	}
	return false;
}

/*SEND TO ALL*/

/* sends the message to all clients and admins in the channel without any exclusions.*/
void		Channel::sendTo_all(std::string rpl1) {
	for(size_t i = 0; i < _admins.size(); i++)
		if(send(_admins[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
	for(size_t i = 0; i < _clients.size(); i++)
		if(send(_clients[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
}

/*Sends the message to all clients and admins in the channel, except for the client with
the specified file descriptor*/
void		Channel::sendTo_all(std::string rpl1, int fd) {
	for(size_t i = 0; i < _admins.size(); i++){
		if(_admins[i].getFd() != fd)
			if(send(_admins[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
	for(size_t i = 0; i < _clients.size(); i++){
		if(_clients[i].getFd() != fd) /// Exclude the client with the specified file descriptor
			if(send(_clients[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
}

int Channel::checkClientExistence(int fd) {
	for (int i = 0; i < _clients.size(); i++) {
		std::cout << "checking = " << fd << std::endl;
		if (_clients[i].getFd() == fd) {
			return (1);
		}
	}
	return (0);
}

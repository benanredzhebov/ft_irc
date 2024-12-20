/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:04:19 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/20 15:26:05 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Channel.hpp"

Channel::Channel() {}

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

/*GETTERS*/
std::string	Channel::getName() {return this->_name;}

/*Consider a chat application where users can join different channels. The server
needs to find a specific client in a channel by their nickname to perform operations
like sending private messages or kicking them out of the channel.*/
Client* Channel::getClientInChannel(std::string name) {
	for (std::vector<Client>::iterator	it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNickName() == name)
			return &(*it);
	}
	return NULL;
}


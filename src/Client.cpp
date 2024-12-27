/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:17:13 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/26 20:33:43 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

/*GETTERS*/

int			Client::getFd() {return this->_fd;}
bool		Client::getRegistered() {return this->_registered;}
bool		Client::getLogedIn() {return this->_logedin;}
std::string Client::getNickName() {return this->_nickname;}
std::string	Client::getUserName() {return this->_username;}

bool		Client::nickGet() {return this->_nickbool;}
/*SETTERS*/
void		Client::nickSet(bool value) {_nickbool = value;}


void		Client::setRegistered(bool value) {_registered = value;}

void		Client::setNickName(std::string &nickname) {this->_nickname = nickname;}

void		Client::setUsername(std::string &username) {this->_username = username;}

void		Client::setLogedIn(bool value) {this->_logedin = value;}

void		Client::setClientFd(int fd) {_fd = fd;}

// std::string	Client::getHostname() {
// 	std::string hostname = this->getNickName() + "!" + this->getUserName();
// 	return hostname;
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:17:13 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/20 15:37:23 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

/*GETTERS*/

int			Client::getFd() {return this->_fd;}
bool		Client::getRegistered() {return this->_registered;}
bool		Client::getLogedIn() {return this->_logedin;}
std::string Client::getNickName() {return this->_nickname;}
std::string	Client::getUserName() {return this->_username;}

/*SETTERS*/
void		Client::setRegistered(bool value) {_registered = value;}

void		Client::setNickName(std::string &nickname) {this->_nickname = nickname;}

void		Client::setLogedIn(bool value) {this->_logedin = value;}
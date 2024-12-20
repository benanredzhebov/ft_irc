/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authenti.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:00:09 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/20 15:37:34 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*PASS COMMAND
handles client authentication based on a command received from the client
If the command is valid and the password matches, the client is marked as registered*/

void	Server::client_authen(int fd, std::string cmd) {
	Client *cli = getClient(fd);
	cmd = cmd.substr(4);
	size_t	pos = cmd.find_first_not_of("\t\v ");
	if (pos < cmd.size()) {
		cmd = cmd.substr(pos);
		if (cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	if (pos == std::string::npos || cmd.empty())
		sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
	else if(!cli->getRegistered()) {
		std::string	pass = cmd;
		if (pass == _password)
			cli->setRegistered(true);
		else
			sendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
		sendResponse(ERR_ALREADYREGISTERED(getClient(fd)->getNickName()), fd);
}

/*NICK COMMAND
Check if a given nickname is valid*/

bool	Server::is_validNickname(std::string &nickname) {
	if (!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for (size_t i = 1; i < nickname.size(); i++) {
		if (!std::isalnum(nickname[i]) && nickname[i] != '_')
			return false;
	}
	return true;
}

/*checks if a given nickname is already in use by any of the connected clients*/
bool	Server::nickNameInUse(std::string &nickname) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getNickName() == nickname)
			return true;
	}
	return false;
}

void	Server::set_nickname(std::string cmd, int fd) {
	std::string	inuse;
	cmd = cmd.substr(4);
	size_t	pos = cmd.find_last_not_of("\t\v ");
	if (pos < cmd.size()) {
		cmd = cmd.substr(pos);
		if (cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	Client	*cli = getClient(fd);
	if (pos == std::string::npos || cmd.empty())
		{sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd); return ;}
	if (nickNameInUse(cmd) && cli->getNickName() != cmd) {
		inuse = "*";
		if (cli->getNickName().empty())
			cli->setNickName(inuse);
		sendResponse(ERR_NICKINUSE(std::string(cmd)), fd);
		return;
	}
	if (!is_validNickname(cmd)) {
		sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return;
	}
	else {
		if(cli && cli->getRegistered())
		{
			std::string oldnick = cli->getNickName();
			cli->setNickName(cmd);
			for(size_t i = 0; i < _channels.size(); i++){
				Client *cl = _channels[i].getClientInChannel(oldnick);
				if(cl)
					cl->setNickName(cmd);
			}
			if(!oldnick.empty() && oldnick != cmd)
			{
				if(oldnick == "*" && !cli->getUserName().empty())
				{
					cli->setLogedIn(true);
					sendResponse(RPL_CONNECTED(cli->getNickName()), fd); //connected to IRC server
					sendResponse(RPL_NICKCHANGE(cli->getNickName(),cmd), fd);
				}
				else
					sendResponse(RPL_NICKCHANGE(oldnick,cmd), fd);
				return;
			}
			
		}
		else if (cli && !cli->getRegistered())
			sendResponse(ERR_NOTREGISTERED(cmd), fd);
	}
	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*" && !cli->getLogedIn())
	{
		cli->setLogedIn(true);
		sendResponse(RPL_CONNECTED(cli->getNickName()), fd);
	}
}
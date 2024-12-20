/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authenti.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:00:09 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/20 11:41:45 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*PASS CMD
handles client authentication based on a command received from the client
If the command is valid and the password matches, the client is marked as registered*/

void    Server::client_authen(int fd, std::string cmd) {
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


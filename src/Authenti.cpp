/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authenti.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:00:09 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/26 17:09:26 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*PASS COMMAND
handles client authentication based on a command received from the client
If the command is valid and the password matches, the client is marked as registered*/

int	Server::clientPasswordVerify(Client *cli, std::vector<std::string>	splited_cmd) {
	if (splited_cmd.size() == 2) {
		if (splited_cmd[0] == "PASS") {
			if (!cli->getRegistered()) {
				if (_password == splited_cmd[1]) {
					std::cout << "[ " << cli->getFd() << " ] " << "password confirmed\n" << std::endl;
					cli->setRegistered(true);
					sendResponse(G_PASSWORD, cli->getFd()); 
					return (1);
				}
				else
					sendResponse(ERR_INCORPASS(splited_cmd[1]), cli->getFd());
			}
			else
				sendResponse(ERR_ALREADYREGISTERED(cli->getNickName()), cli->getFd());
		}
	}
	cli->decrementPasswordTrials();
	if (cli->getPasswordTrials() <= 0) {
		epoll_ctl(epfd, EPOLL_CTL_DEL, cli->getFd(), 0);
		removeClient(cli->getFd());
	} 
	return (0);
}

int	Server::confirmClientInfo(Client *cli) {
	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && !cli->getLogedIn())
	{
		cli->setLogedIn(true);
		sendResponse(RPL_CONNECTED(cli->getNickName()), cli->getFd());
	}	
	return (1);
}

int Server::clientNickName(Client *cli, std::vector<std::string> splited_cmd) {
	if (!cli->getRegistered())
		return (0);
	if (splited_cmd.size() == 2) {
		if (splited_cmd[0] == "NICK") {
			if (cli->getRegistered()) {
				if (set_nickname(splited_cmd[1], cli))
					return (1);
			}
		}
		else {
			sendResponse(ERR_NICKNAME_NEEDED(splited_cmd[0]), cli->getFd());
			return (0);
		}
	}
	else {
		sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), cli->getFd());
		return (0);
	}
	return (0);
}

int Server::clientUserName(Client *cli, std::vector<std::string> splited_cmd) {
	if (!cli->nickGet())
		return (0);

	if (splited_cmd.size() == 5) {
		if (splited_cmd[0] == "USER") {
			if (cli->getRegistered()) {
				if (set_username(splited_cmd, cli)) {
					confirmClientInfo(cli);
					return (1);
				}
			}
		}
	}
	else {
		sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), cli->getFd());
		return (0);
	}
	return (0);
}

/*Consider a chat application where users need to set or change their nicknames.
The server needs to handle this request, ensuring the nickname is valid and not already in use.*/
int	Server::set_nickname(std::string cmd, Client *cli) {
	
	if (!cli && cmd.empty())
		{sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), cli->getFd()); return 0;}
	if (nickNameInUse(cmd)) {
		sendResponse(ERR_NICKINUSE(std::string(cmd)), cli->getFd());
		return (0);
	}
	if (!is_validNickname(cmd)) {
		sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), cli->getFd());
		return (0);
	}
	if(cli && cli->getRegistered()) {
		if (cli->nickGet()) {
			std::string old_nick = cli->getNickName();
			cli->setNickName(cmd);
			sendResponse(RPL_NICKCHANGE(old_nick, cli->getNickName()), cli->getFd());
		}
		cli->nickSet(true);
		cli->setNickName(cmd);
		sendResponse(NICK_SET, cli->getFd());
		return (1);
	}
	return (0);
}


int	Server::set_username(std::vector<std::string> splited_cmd, Client *cli)
{
	if((splited_cmd.empty() || splited_cmd.size() != 5)) {
		std::cout << "from username\n" << std::endl;
		sendResponse(ERR_NOTENOUGHPARAM(cli->getNickName()), cli->getFd());
		return (0); 
	}
	if (userNameInUse(splited_cmd[1])) {
		sendResponse(ERR_NICKINUSE(std::string(splited_cmd[1])), cli->getFd());
		return (0);
	}
	if (!is_validUserName(splited_cmd[1])) {
		sendResponse(ERR_ERRONEUSNICK(std::string(splited_cmd[1])), cli->getFd());
		return (0);
	}
	if(cli && cli->getRegistered()) {
		cli->setUsername(splited_cmd[1]);
		sendResponse(USER_SET, cli->getFd());
		return (1);
	}
	return (0);
}

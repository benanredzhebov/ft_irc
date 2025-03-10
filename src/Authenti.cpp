/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authenti.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:00:09 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/14 11:00:22 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*PASS COMMAND/
handles client authentication based on a command received from the client
If the command is valid and the password matches, the client is marked as registered*/

// int Server::clientPasswordVerify(Client *cli, std::vector<std::string> splited_cmd) {
// 	if (splited_cmd.size() != 2) {
// 		sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), cli->getFd());
// 		return 0;
// 	}

// 	if (splited_cmd[0] == "PASS") {
// 		if (!cli->getRegistered()) {
// 			if (_password == splited_cmd[1]) {
// 				std::cout << GRE << "Client fd [" << cli->getFd() << "] password authenticated" << RESET << std::endl;
// 				cli->setRegistered(true);
// 				sendResponse(G_PASSWORD, cli->getFd());
// 				return 1;
// 			} else {
// 				sendResponse(ERR_INCORPASS(splited_cmd[1]), cli->getFd());
// 				cli->decrementPasswordTrials();
// 				if (cli->getPasswordTrials() <= 0) {
// 					removeClientInstance(cli->getFd());
// 				}
// 				return 0;
// 			}
// 		} else {
// 			sendResponse(ERR_ALREADYREGISTERED(cli->getNickName()), cli->getFd());
// 			return 0;
// 		}
// 	} else {
// 		sendResponse(ERR_UNKNOWNCOMMAND(splited_cmd[0]), cli->getFd());
// 		return 0;
// 	}
// }

int Server::clientPasswordVerify(Client *cli, std::vector<std::string> splited_cmd) {
	if (splited_cmd.size() < 2 || splited_cmd.size() > 2) {
		sendResponse(ERR_NOTENOUGHPARAM(splited_cmd[0]), cli->getFd());
		return 0;
	}

	if (splited_cmd[0] != "PASS") {
		sendResponse(ERR_UNKNOWNCOMMAND(splited_cmd[0]), cli->getFd());
	}

	if (splited_cmd[0] == "PASS") {
		if (!cli->getRegistered()) {
			if (_password == splited_cmd[1]) {
				std::cout << GRE << "Client fd [" << cli->getFd() << "] password authenticated" << RESET << std::endl;
				cli->setRegistered(true);
				sendResponse(G_PASSWORD, cli->getFd());
				return 1;
			} else {
				sendResponse(ERR_INCORPASS(splited_cmd[1]), cli->getFd());
				cli->decrementPasswordTrials();
				if (cli->getPasswordTrials() <= 0) {
					removeClientInstance(cli->getFd());
				}
				return 0;
			}
		} else {
			sendResponse(ERR_ALREADYREGISTERED(cli->getNickName()), cli->getFd());
			return 0;
		}
	} else {
		sendResponse(ERR_UNKNOWNCOMMAND(splited_cmd[0]), cli->getFd());
		return 0;
	}
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
	if (!cli || !cli->getRegistered()) {
		return 0;
	}

	if (splited_cmd.size() == 2) {
		if (splited_cmd[0] == "NICK") {
			if (set_nickname(splited_cmd[1], cli)) {
				if (!confirmClientInfo(cli))
					return (0);
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
	std::vector<std::string> temp_vec;

	if((splited_cmd.size() < 5)) {
		sendResponse(ERR_NOTENOUGHPARAM(cli->getNickName()), cli->getFd());
		return (0); 
	}	
	if (splited_cmd[0] == "USER") {
		if (splited_cmd[4][0] == ':' || splited_cmd[4] == ":") {
			for (int i = 0; i < 4; i++) {
				temp_vec.push_back(splited_cmd[i]);
			}
			std::string temp = getColonMessage(4, splited_cmd);
			temp_vec.push_back(temp);
		}
		else if (splited_cmd.size() == 5)
			temp_vec = splited_cmd;
		if (cli->getRegistered()) {
			if (set_username(temp_vec, cli)) {
				if (!confirmClientInfo(cli))
					return (0);
				return (1);
			}
		}
	}
	sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), cli->getFd());
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
	if(cli && cli->getRegistered() && !cli->getUserstate()) {
		cli->setUserstate(true);
		cli->setUsername(splited_cmd[1]);
		sendResponse(USER_SET, cli->getFd());
		return (1);
	}
	return (0);
}

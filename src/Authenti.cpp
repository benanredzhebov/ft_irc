/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authenti.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:00:09 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/23 21:55:48 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*PASS COMMAND
handles client authentication based on a command received from the client
If the command is valid and the password matches, the client is marked as registered*/

int	Server::client_authen(int fd) {
	int			count = 3;
	Client		*cli;

	cli = getClient(fd);
	if (cli != NULL) {
		std::cout <<"most probably client has been registed\n\n" << std::endl;
		return (0);
	}
	while (count > 0){
		sendResponse(E_PASSWORD, fd);
		if (!recievePasswordVerify(fd)){
			count--;
			continue;
		}
		else {
			return (1);
		}
	}
	return (-1);
	//most likely not yet pushed so i should find a way to check and then push.
}

int	Server::clientInfoSave(int fd) {
	Client		*cli;
	std::string	userName;
	std::string	nickName;

	cli = getClient(fd);
	if (!cli) {
		std::cout << "defff an issue \n" << std::endl;
	}
	while (1) {
		sendResponse(NICKNAME, fd);
		nickName = receiveSetNameUname(fd);
		if (set_username(nickName, fd))
			break ;
	}
	while (1) {
		sendResponse(USERNAME, fd);
		userName = receiveSetNameUname(fd);
		if (set_nickname(userName, fd))
			break ;
	}
	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && !cli->getLogedIn())
	{
		cli->setLogedIn(true);
		sendResponse(RPL_CONNECTED(cli->getNickName()), fd);
	}	
	return (1);
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

bool	Server::is_validUserName(std::string &username) {
	if (!username.empty() && (username[0] == '&' || username[0] == '#' || username[0] == ':'))
		return false;
	for (size_t i = 1; i < username.size(); i++) {
		if (!std::isalnum(username[i]) && username[i] != '_')
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

//not used yet in the set_userNmae
bool	Server::userNameInUse(std::string &username) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getUserName() == username)
			return true;
	}
	return false;
}

/*Consider a chat application where users need to set or change their nicknames.
The server needs to handle this request, ensuring the nickname is valid and not already in use.*/
int	Server::set_nickname(std::string cmd, int fd) {
	Client		*cli;
	
	cli = getClient(fd);
	if (!cli && cmd.empty())
		{sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd); return 0;}
	if (nickNameInUse(cmd)) {
		sendResponse(ERR_NICKINUSE(std::string(cmd)), fd);
		return (0);
	}
	if (!is_validNickname(cmd)) {
		sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return (0);
	}
	if(cli && cli->getRegistered()) {
		cli->setNickName(cmd);
		sendResponse(NICK_SET, fd);
		return (1);
	}
	return (0);
}


int	Server::set_username(std::string cmd, int fd)
{
	Client	*cli;

	cli = getClient(fd);
	if (!cli && cmd.empty())
		{sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd); return 0;}
	if (userNameInUse(cmd)) {
		sendResponse(ERR_NICKINUSE(std::string(cmd)), fd);
		return (0);
	}
	if (!is_validUserName(cmd)) {
		sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return (0);
	}
	if(cli && cli->getRegistered()) {
		cli->setUsername(cmd);
		sendResponse(USER_SET, fd);
		return (1);
	}
	return (0);
}

/*USER COMMAND
Set the username after connecting. The server need to handle this request, ensuring
the username is provided and the client is registered.*/
// void	Server::set_username(std::string cmd, int fd)
// {
// 	std::vector<std::string> splited_cmd = split_cmd(cmd);
	
// 	Client *cli = getClient(fd);
// 	if((cli && splited_cmd.size() < 5)) // Check for the missing parameters
// 		{sendResponse(ERR_NOTENOUGHPARAM(cli->getNickName()), fd); return; }
// 	if(!cli  || !cli->getRegistered()) // Check if client is registered
// 		sendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
// 	else if (cli && !cli->getUserName().empty()) // Check if username is already set
// 		{sendResponse(ERR_ALREADYREGISTERED(cli->getNickName()), fd); return;}
// 	else
// 		cli->setUsername(splited_cmd[1]); // Set username
// 	// Check if client is fully loged in
// 	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*"  && !cli->getLogedIn())
// 	{
// 		cli->setLogedIn(true);
// 		sendResponse(RPL_CONNECTED(cli->getNickName()), fd);
// 	}
// }
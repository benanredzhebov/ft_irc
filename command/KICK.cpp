/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 10:00:59 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/20 11:32:25 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void FindK(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind) break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string SplitCmdK(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, reason;
	int count = 3;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 3) return std::string("");
	FindK(cmd, tmp[2], reason);
	return reason;
}

std::string Server::splitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd)
{
	std::string reason = SplitCmdK(cmd, tmp);
	if (tmp.size() < 3) // check if the client send the channel name and the client to kick
		return std::string("");
	tmp.erase(tmp.begin());
	std::string str = tmp[0]; std::string str1;
	user = tmp[1]; tmp.clear();
	for (size_t i = 0; i < str.size(); i++){//split the first string by ',' to get the channels names
		if (str[i] == ',')
			{tmp.push_back(str1); str1.clear();}
		else str1 += str[i];
	}
	tmp.push_back(str1);
	for (size_t i = 0; i < tmp.size(); i++)
		{if (tmp[i].empty())tmp.erase(tmp.begin() + i--);}
	if (reason[0] == ':') reason.erase(reason.begin());
	else 
		{for (size_t i = 0; i < reason.size(); i++){if (reason[i] == ' '){reason = reason.substr(0, i);break;}}}
	return reason;
}

Channel* Server::getChannel(std::string name) {
    for (size_t i = 0; i < this->_channels.size(); i++) {
        if (this->_channels[i].getName() == name)
            return &_channels[i];
    }
    return NULL;
}

void	Server::KICK(std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string reason ,user;
	reason = splitCmdKick(cmd, tmp, user, fd);
	if (user.empty()) {
		sendResponse(ERR_NOTENOUGHPARAM(getClient(fd)->getNickName()) , fd);
		return;
	}
	for (size_t i = 0; i < tmp.size(); i++){
		if (getChannel(tmp[i])){
			Channel *ch = getChannel(tmp[i]);
			if (!ch->get_client(fd) && !ch->get_admin(fd)) {
				sendResponse(ERR_NOTONCHANNEL(getClient(fd)->getNickName(), tmp[i]) , fd);
				continue;
			}
			if(ch->get_admin(fd)) { 
				if (ch->getClientInChannel(user)) {
					std::stringstream ss;
					ss << ":" << getClient(fd)->getNickName() << "!~" << getClient(fd)->getUserName() << "@" << "localhost" << " KICK " << tmp[i] << " " << user;
					if (!reason.empty())
						ss << " :" << reason << "\r\n";
					else ss << "\r\n";
					if (ch->get_admin(ch->getClientInChannel(user)->getFd()))
						ch->remove_admin(ch->getClientInChannel(user)->getFd());
					else
						ch->remove_client(ch->getClientInChannel(user)->getFd());
					sendResponse(CLIKICKOUT(getClient(fd)->getUserName(), user, tmp[i], reason), getClientNick(user)->getFd());
					ch->sendTo_all(ss.str());
					if (ch->getClientsNumber() == 0)
						_channels.erase(_channels.begin() + i);
				}
				else {
					sendResponse(ERR_USERNOTINCHANNEL(getClient(fd)->getNickName(), tmp[i]), fd);
					continue;
				}
			}
			else {
				sendResponse(ERR_CHANOPRIVSNEEDED(getClient(fd)->getNickName(), tmp[i]), fd);
				continue;
			}
		}
		else
		{
			std::cout << "from last end\n\n\n";
			sendResponse(ERR_NOSUCHCHANNEL(getClient(fd)->getNickName(), tmp[i]) , fd);
		}
	}
}
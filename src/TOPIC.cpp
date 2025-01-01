/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:32:22 by danevans          #+#    #+#             */
/*   Updated: 2025/01/01 02:52:15 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

// where should the topic be called from ? server or from the channel
// called from the server , the server firsr verify if the mode is set to on or off and then the sever does the whole call
// checking fist if the client is the admin and then it can set the topic or thhe bool is on and anyonne can set the topic

std::string getColonMessage(int x, std::vector<std::string> str) {
	std::string temp;
	int	flag = 0;
	while (x < str.size()){
		if (!flag && (str[x][0] == ':' ||  str[x] == ":")) {
			flag = 1;
		}
		if (flag && !str[x].empty()) {
			temp += " " + str[x];
		}
		x++;
	}
	return (temp);
}

void	Server::TOPIC(std::vector<std::string> splited_cmd, Client *client){
	if(splited_cmd.size() < 2) { 
		sendResponse(ERR_NOTENOUGHPARAM(splited_cmd[0]) , client->getFd());
		return;
	}
	if(splited_cmd[1][0] != '#' || !getChannel(splited_cmd[1])) {
		sendResponse(ERR_NOSUCHCHANNEL(client->getNickName(), splited_cmd[1]) , client->getFd());
		return;
	}
	// here should ensure that client is either on the channel , admin or everyine can send
	// both should be combined as one if !admnin || !open to everyone(clients in the channel) .
	if (!(getChannel(splited_cmd[1])->get_admin(client->getFd())))	{
		sendResponse(ERR_NOTONCHANNEL(client->getNickName(), splited_cmd[1]) , client->getFd());
		return;
	}
	Channel *channel;
	channel = getChannel(splited_cmd[1]);
	
	if (splited_cmd.size() == 2 && splited_cmd[1][0] == '#') {
		sendResponse(channel->getTopicName(), client->getFd());
		return ;
	}
	else {
		std::string topic = getColonMessage(2, splited_cmd);
		channel->setTopicName(topic);
		if (channel->checkClientExistence(client->getFd())) {
			channel->sendTo_all(CHANTOPIC(client->getNickName(), splited_cmd[1], channel->getTopicName()), client->getFd());
		}
	}
	return ;
}




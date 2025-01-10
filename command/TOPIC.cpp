/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:32:22 by danevans          #+#    #+#             */
/*   Updated: 2025/01/09 12:54:39 by beredzhe         ###   ########.fr       */
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

void Server::TOPIC(std::string& cmd, int fd) {
	std::string channelName;
	std::string topic;
	Channel* channel;
	Client* cli = getClient(fd);

	if (!cli) {
		std::cerr << "Error: Client not found for fd: " << fd << std::endl;
		return;
	}

	size_t found = cmd.find_first_not_of("TOPICtopic \t\b");
	if (found != std::string::npos)
		cmd = cmd.substr(found);
	else {
		sendResponse(ERR_NEEDMOREPARAMS(std::string("*")), cli->getFd());
		return;
	}
	
	std::stringstream ss(cmd);
	ss >> channelName;
	size_t colonPos = cmd.find(':');
	
	if (colonPos != std::string::npos) 
		topic = cmd.substr(colonPos + 1);

	if (!topic.empty() && topic[0] == ' ')
		topic = topic.substr(1);

	if (channelName.empty() || channelName[0] != '#' || !(channel = getChannel(channelName))) {
		sendResponse(ERR_NOSUCHCHANNEL(cli->getNickName(), channelName), fd);
		return;
	}

	if (!channel->clientInChannel(cli->getNickName())) {
		sendResponse(ERR_NOTONCHANNEL(cli->getNickName(), channelName), fd);
		return;
	}

	if (topic.empty()) {
		// Get the topic
		std::string currentTopic = channel->getTopic();
		if (currentTopic.empty()) {
			sendResponse(RPL_NOTOPIC(cli->getNickName(), channelName), fd);
		} else {
			sendResponse(RPL_TOPIC(cli->getNickName(), channelName, currentTopic), fd);
		}
	} else {
		// Set the topic
		if (channel->getModeAtindex(1) && !channel->get_admin(fd)) {
			sendResponse(ERR_CHANOPRIVSNEEDED(cli->getNickName(), channelName), fd);
			return;
		}
		channel->setTopic(topic);
		sendResponse(RPL_TOPIC(cli->getNickName(), channelName, topic), fd);
		channel->sendTo_all(":" + cli->getNickName() + " TOPIC " + channelName + " " + topic + "\r\n");
	}
}
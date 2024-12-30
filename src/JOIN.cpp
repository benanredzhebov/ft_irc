/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 12:58:48 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/30 20:49:50 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int Server::splitJoin(std::vector<std::pair<std::string, std::string> > &token, 
                      const std::vector<std::string> splited_cmd) {
    std::vector<std::string> channels;
    std::vector<std::string> passwords;
    std::vector<std::string> uniqueChannels;
    std::string channelStr, passwordStr;

    for (size_t i = 1; i < splited_cmd.size(); ++i) {
		std::string part = splited_cmd[i];
		if ((part[0] == '#' || part[0] == '&')) {
			channelStr = part;
			if (!channelStr.empty() && channelStr[channelStr.size() - 1] == ',') {
				channelStr.erase(channelStr.size() - 1);
			}
			if (std::find(uniqueChannels.begin(), uniqueChannels.end(), channelStr) == uniqueChannels.end()) {
				uniqueChannels.push_back(channelStr);
				channels.push_back(channelStr);
			}
		} else {
			passwordStr = part;
			if (!passwordStr.empty() && passwordStr[passwordStr.size() - 1] == ',') {
				passwordStr.erase(passwordStr.size() - 1); // Remove trailing comma
			}
			passwords.push_back(passwordStr);
		}
	}
		if (channels.empty() || passwords.size() > channels.size()) {
		    std::cout << "Place holder here Error: No valid channels found.\n";
		    return (0);
		}
		for (size_t i = 0; i < channels.size(); ++i) {
		    if (i >= passwords.size()) {
		        passwordStr = "";
		    } else {
		        passwordStr = passwords[i];
		    }		
		    token.push_back(std::make_pair(channels[i], passwordStr));
		}		
		return (1); 
}

bool IsInvited(Client *cli, std::string ChName, int flag){
	if(cli->getInviteChannel(ChName)){
		if (flag == 1)
			cli->rmChannelInvite(ChName);
		return true;
	}
	return false;
}

void Server::existCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd)
{
	if (this->_channels[j].getClientInChannel(getClient(fd)->getNickName()))
		return ;
	if (searchForClients(getClient(fd)->getNickName()) >= 10) {
		sendResponse(ERR_TOOMANYCHANNELS(getClient(fd)->getNickName()), fd);
		return ;
	}
	if (!this->_channels[j].GetPassword().empty() && this->_channels[j].GetPassword() != token[i].second) {
		if (!IsInvited(getClient(fd), token[i].first, 0)) {
			sendResponse(ERR_BADCHANNELKEY(getClient(fd)->getNickName(), token[i].first), fd);
			return ;
		}
	}
	if (this->_channels[j].getInvitOnly()){
		if (!IsInvited(getClient(fd), token[i].first, 1)) {	
			sendResponse(ERR_INVITEONLYCHAN(getClient(fd)->getNickName(), token[i].first), fd);
			return ;
		}
	}
	if (this->_channels[j].getLimit() && this->_channels[j].getClientsNumber() >= this->_channels[j].getLimit()) {
		sendResponse(ERR_CHANNELISFULL(getClient(fd)->getNickName(), token[i].first), fd);
		return ;
	}
	Client *cli = getClient(fd);
	this->_channels[j].add_client(*cli);
	if(_channels[j].getTopicName().empty())
		sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first) + \
			RPL_NAMREPLY(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(getClient(fd)->getNickName(),_channels[j].getName()),fd);
	else
		sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first) + \
			RPL_TOPICIS(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].getTopicName()) + \
			RPL_NAMREPLY(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(getClient(fd)->getNickName(),_channels[j].getName()),fd);
    _channels[j].sendTo_all(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first), fd);
}

void Server::newChannelCreate(std::string chName, std::string chPass, Client *client)
{
	if (client->getChannelSize() >= 10) {
		std::cerr << "place holder for the max chanel and then return\n";
		return ;
	}
	Channel newChannel;
	newChannel.setName(chName);
	if (chPass != "")
		newChannel.setPassword(chPass);
	// newChannel.add_client(*client); 
	newChannel.add_admin(*client); // ensure the admin has been added to the channel
	newChannel.set_createiontime(); //what was this ever used for ???
	_channels.push_back(newChannel);
    sendResponse(RPL_JOINMSG(client->getHostname(), client->getIpAdd(), chName), client->getFd());
    sendResponse(RPL_NAMREPLY(client->getNickName(), chName, newChannel.clientChannel_list()), client->getFd());
    sendResponse(RPL_ENDOFNAMES(client->getNickName(), chName), client->getFd());
}

int	Server::JOIN(std::vector<std::string> splited_cmd, Client *client) {
	std::vector<std::pair<std::string, std::string> > token;

	if (!splitJoin(token, splited_cmd)) {
		sendResponse(ERR_NEEDMOREPARAMS(splited_cmd[0]), client->getFd());
		return (0);
	}
	if (DEBUG) {
		for (size_t i = 0; i < token.size(); ++i) {
			std::cout << "Key: " << token[i].first << "\n ";
			std::cout << "Value: " << token[i].second << std::endl;
		}
	}
	for (size_t i = 0; i < token.size(); i++){
		bool flag = false;
		for (size_t j = 0; j < this->_channels.size(); j++){
			if (this->_channels[j].getName() == token[i].first){
				existCh(token, i, j, client->getFd());
				flag = true;
				break;
			}
		}
		if (!flag)
			newChannelCreate(token[i].first, token[i].second, client);
	}
	return (1);
}
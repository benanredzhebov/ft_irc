/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 12:58:48 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/27 19:25:27 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*It parses the JOIN command to extract channel names and passwords
RFC 2812 JOIN <channel>{,<channel>} [<key>{,<key>}] */

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

// int Server::searchForClients(std::string nickname)
// {
// 	int count = 0;
// 	for (size_t i = 0; i < this->_channels.size(); i++)
// 	{
// 		if (this->_channels[i].getClientInChannel(nickname))
// 			count++;
// 	}
// 	return count;
// }

// bool IsInvited(Client *cli, std::string ChName, int flag){
// 	if(cli->getInviteChannel(ChName)){
// 		if (flag == 1)
// 			cli->rmChannelInvite(ChName);
// 		return true;
// 	}
// 	return false;
// }

// void Server::existCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd)
// {
// 	if (this->_channels[j].getClientInChannel(getClient(fd)->getNickName()))// if the client is already registered
// 		return;
// 	if (searchForClients(getClient(fd)->getNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
// 		{senderror(405, getClient(fd)->getNickName(), getClient(fd)->getFd(), " :You have joined too many channels\r\n"); return;}
// 	if (!this->_channels[j].GetPassword().empty() && this->_channels[j].GetPassword() != token[i].second){// ERR_BADCHANNELKEY (475) // if the password is incorrect
// 		if (!IsInvited(getClient(fd), token[i].first, 0))
// 			{senderror(475, getClient(fd)->getNickName(), "#" + token[i].first, getClient(fd)->getFd(), " :Cannot join channel (+k) - bad key\r\n"); return;}
// 	}
// 	if (this->_channels[j].getInvitOnly()){// ERR_INVITEONLYCHAN (473) // if the channel is invit only
// 		if (!IsInvited(getClient(fd), token[i].first, 1))
// 			{senderror(473, getClient(fd)->getNickName(), "#" + token[i].first, getClient(fd)->getFd(), " :Cannot join channel (+i)\r\n"); return;}
// 	}
// 	if (this->_channels[j].getLimit() && this->_channels[j].getClientsNumber() >= this->_channels[j].getLimit())// ERR_CHANNELISFULL (471) // if the channel reached the limit of number of clients
// 		{senderror(471, getClient(fd)->getNickName(), "#" + token[i].first, getClient(fd)->getFd(), " :Cannot join channel (+l)\r\n"); return;}
// 	// add the client to the channel
// 	Client *cli = getClient(fd);
// 	this->_channels[j].add_client(*cli);
// 	if(_channels[j].getTopicName().empty())
// 		sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first) + \
// 			RPL_NAMREPLY(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].clientChannel_list()) + \
// 			RPL_ENDOFNAMES(getClient(fd)->getNickName(),_channels[j].getName()),fd);
// 	else
// 		sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first) + \
// 			RPL_TOPICIS(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].getTopicName()) + \
// 			RPL_NAMREPLY(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].clientChannel_list()) + \
// 			RPL_ENDOFNAMES(getClient(fd)->getNickName(),_channels[j].getName()),fd);
//     _channels[j].sendTo_all(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first), fd);
// }



	// signal(SIGINT, SignalHandler);
	// signal(SIGQUIT, SignalHandler);

// void SignalHandler(int signum){
// 	(void)signum;
// 	std::cout << std::endl << "Signal Received!" << std::endl;
// 	_sendMessage("QUIT\r\n", ircsock);
// }


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
	newChannel.add_admin(*client);
	newChannel.set_createiontime(); //what was this ever used for ???
	_channels.push_back(newChannel);
    sendResponse(RPL_JOINMSG(client->getHostname(), client->getIpAdd(), chName), client->getFd());
	// i dont know what clientChannel_list is used for  here
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
	
	if (token.size() > 10) {
		std::cout << "place holder for error for too many channel\n";
		// senderror(407, client->getNickName(), getClient(fd)->getFd(), " :Too many channels\r\n");
		return (0);
	}
	for (size_t i = 0; i < token.size(); i++){
		bool flag = false;
		for (size_t j = 0; j < this->_channels.size(); j++){
			if (this->_channels[j].getName() == token[i].first){
				existCh(token, i, j, fd);
				flag = true;
				break;
			}
		}
		if (!flag)
			newChannelCreate(token[i].first, token[i].second, client);
	}
	return (1);
}
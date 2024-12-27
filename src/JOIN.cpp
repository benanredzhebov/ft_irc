/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 12:58:48 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/27 16:14:35 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*It parses the JOIN command to extract channel names and passwords
RFC 2812 JOIN <channel>{,<channel>} [<key>{,<key>}] */
int Server::splitJoin(std::vector<std::pair<std::string, std::string> > &token, std::vector<std::string> splited_cmd)
{ //maybe we should for key for invalid or not . you decide
	std::vector<std::string> channels;
	std::vector<std::string> passwords;
	std::string channelStr, passwordStr;

	for (int i = 1; i < splited_cmd.size(); i++) {
		channelStr = splited_cmd[i];
		if ((channelStr[0] == '#' || channelStr[0] == '&')) {
    		channelStr = channelStr.substr(1); 
			if ((channelStr[channelStr.size() - 1]) == ',') {
			channelStr = channelStr.substr(0, channelStr.size() - 1);
		}
		channels.push_back(channelStr); 
		}
	}
	if (channels.empty())
		return (0);
	for (int i = 1; i < splited_cmd.size(); i++) {
		passwordStr = splited_cmd[i];
		if ((passwordStr[0] != '#' && passwordStr[0] != '&')) {
			if ((passwordStr[passwordStr.size() - 1]) == ',') {
			passwordStr = passwordStr.substr(0, passwordStr.size() - 1);
		}
		passwords.push_back(passwordStr); 
		}
	}
	if (passwords.size() > channels.size()) {
		std::cout << "key exceed channel need to find error code to match when pasword is more that channel\n";
		return (0); 
	}
	for (size_t i = 0; i < channels.size(); ++i) {
		if (i >= passwords.size()) {
			passwordStr = "";
			passwords.push_back(passwordStr);
		}
		token.push_back(std::make_pair(channels[i], passwords[i]));
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


// void Server::notExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd)
// {
// 	if (searchForClients(getClient(fd)->getNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
// 		{senderror(405, getClient(fd)->getNickName(), getClient(fd)->getFd(), " :You have joined too many channels\r\n"); return;}
// 	Channel newChannel;
// 	newChannel.setName(token[i].first);
// 	newChannel.add_admin(*getClient(fd));
// 	newChannel.set_createiontime();
// 	this->_channels.push_back(newChannel);
// 	// notifiy thet the client joined the channel
//     sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),newChannel.getName()) + \
//         RPL_NAMREPLY(getClient(fd)->getNickName(),newChannel.getName(),newChannel.clientChannel_list()) + \
//         RPL_ENDOFNAMES(getClient(fd)->getNickName(),newChannel.getName()),fd);
// }

	// signal(SIGINT, SignalHandler);
	// signal(SIGQUIT, SignalHandler);

// void SignalHandler(int signum){
// 	(void)signum;
// 	std::cout << std::endl << "Signal Received!" << std::endl;
// 	_sendMessage("QUIT\r\n", ircsock);
// }

int	Server::JOIN(std::vector<std::string> splited_cmd, Client *client) {
	std::vector<std::pair<std::string, std::string> > token;

	if (!splitJoin(token, splited_cmd)) {
		sendResponse(ERR_NEEDMOREPARAMS(splited_cmd[0]), client->getFd());
		return (0);
	}
	if (token.size() > 10) {
		std::cout << "error for too many channel\n";
		// senderror(407, client->getNickName(), getClient(fd)->getFd(), " :Too many channels\r\n");
		return (0);
	}
	// for (size_t i = 0; i < token.size(); i++){
	// 	bool flag = false;
	// 	for (size_t j = 0; j < this->_channels.size(); j++){
	// 		if (this->_channels[j].getName() == token[i].first){
	// 			existCh(token, i, j, fd);
	// 			flag = true;
	// 			break;
	// 		}
	// 	}
	// 	if (!flag)
	// 		notExistCh(token, i, fd);
	// }
	for (size_t i = 0; i < token.size(); ++i) {
        std::cout << "Key: " << token[i].first << "\n ";
		std::cout << "Value: " << token[i].second << std::endl;
    }
	return (1);
}
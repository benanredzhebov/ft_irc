#include "../include/Server.hpp"

/*
RPL_INVITING (341)
ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
ERR_CHANOPRIVSNEEDED (482)
ERR_USERONCHANNEL (443)*/
void Server::INVITE(std::vector<std::string> splited_cmd, Client *client)
{
	if(splited_cmd.size() < 3) { 
		sendResponse(ERR_NOTENOUGHPARAM(splited_cmd[0]) , client->getFd());
		return;
	}
	if(splited_cmd[2][0] != '#' || !getChannel(splited_cmd[2])) {
		// std::cout << "cmd = " << splited_cmd[2] << std::endl;
		sendResponse(ERR_NOSUCHCHANNEL(client->getNickName(), splited_cmd[2]) , client->getFd());
		return;
	}
	if (!(getChannel(splited_cmd[2])) && !(getChannel(splited_cmd[2])->get_admin(client->getFd())))	{
		sendResponse(ERR_NOTONCHANNEL(client->getNickName(), splited_cmd[2]) , client->getFd());
		return;
	}

	if (getChannel(splited_cmd[2])->getClientInChannel(splited_cmd[1])) {
		sendResponse(ERR_USERONCHANNEL(client->getNickName(), splited_cmd[1], splited_cmd[2]) , client->getFd());
		return;
	}
	Client *clt = getClientNick(splited_cmd[1]);
	if (!clt)// ERR_NOSUCHNICK (401) if the given nickname is not found
	{
		sendResponse(ERR_NOSUCHNICK(client->getNickName(), splited_cmd[1]), client->getFd());	
		return;
	}
	if (getChannel(splited_cmd[2])->getInvitOnly() && !getChannel(splited_cmd[2])->get_admin(client->getFd())){
		sendResponse(ERR_CHANOPRIVSNEEDED(client->getNickName(), splited_cmd[2]), client->getFd());
		return;
	}
	if (getChannel(splited_cmd[2])->getLimit() && getChannel(splited_cmd[2])->getClientsNumber() >= getChannel(splited_cmd[2])->getLimit()) {
		sendResponse(ERR_CHANNELISFULL(clt->getNickName(), splited_cmd[2]), client->getFd());
		return;
	}
	//method not implemented yet(this would take channel in client class and add all , the remove does same )
	//the invites works partially bc the message is sent to new client but when the new client sends messgas it doesnt go bc of the client doesnt have the channel added
	// clt->addChannelInvite(splited_cmd[2]);
	std::string rep1 = ": 341 "+ client->getNickName()+" "+ clt->getNickName()+" "+ splited_cmd[2] +"\r\n";
	sendResponse(rep1, client->getFd());
	std::string rep2 = ":"+ clt->getHostname() + " INVITE " + clt->getNickName() + " " + splited_cmd[2]+ "\r\n";
	sendResponse(rep2, clt->getFd());
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 09:02:29 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/06 11:37:24 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

std::string Server::mode_toAppend(std::string chain, char opera, char mode)
{
	std::stringstream ss;
	char last = '\0';
	
	for(size_t i = 0; i < chain.size(); i++)
	{
		if(chain[i] == '+' || chain[i] == '-')
			last = chain[i];
	}
	if(last != opera)
		ss << opera << mode;
	else
		ss << mode;
	return ss.str();
}

void	Server::getCmdArgs(std::string cmd, std::string& name, std::string& modeset, std::string &params) {
	std::istringstream	stm(cmd);
	stm >> name;
	stm >> modeset;
	size_t	found = cmd.find_first_not_of(name + modeset + " \t\v");
	if (found != std::string::npos)
		params = cmd.substr(found);
}

std::vector<std::string>	Server::splitParams(std::string params) {
	if (!params.empty() && params[0] == ':')
		params.erase(params.begin());
	std::vector<std::string>	tokens;
	std::string	param;
	std::istringstream	stm(params);
	while (std::getline(stm, param, ',')) {
		tokens.push_back(param);
	}
	return tokens;
}

void	Server::MODE(std::string& cmd, int fd) {
	std::string			channelName;
	std::string			params;
	std::string			modeset;
	std::stringstream	mode_chain;
	std::string			arguments = ":";
	Channel				*channel;
	char				opera = '\0';

	arguments.clear();
	mode_chain.clear();
	Client *cli = getClient(fd);
	if (!cli) {
		std::cerr << "Error: Client not found for fd: " << fd << std::endl;
	}
	
	size_t	found = cmd.find_first_not_of("MODEmode \t\b");
	if (found != std::string::npos)
		cmd = cmd.substr(found);
	else {
		sendResponse(ERR_NOTENOUGHPARAM(cli->getNickName()), fd);
		return ;
	}
	
	getCmdArgs(cmd, channelName, modeset, params);
	std::cout << "cmd = " << cmd << std::endl;
	std::cout << "chnnlname = " << channelName << std::endl;
	std::cout << "modeset = " << modeset << std::endl;
	std::cout << "params = " << params << std::endl;
	std::vector<std::string>	tokens = splitParams(params);
	if (channelName.empty() || channelName[0] != '#' || !(channel = getChannel(channelName))) {
		sendResponse(ERR_CHANNELNOTFOUND(cli->getUserName(), channelName), fd);
		return ;
	}
	  if (!channel->get_client(fd) && !channel->get_admin(fd)) {
		std::stringstream ss;
		ss << ":localhost 442 " << cli->getNickName() << " " << channelName << " :You are not on that channel\r\n";
		sendResponse(ss.str(), cli->getFd());
		return;
	}
	else if (modeset.empty()) {
		sendResponse(RPL_CHANNELMODES(cli->getNickName(), channel->getName(), channel->getModes()) + \
		RPL_CREATIONTIME(cli->getNickName(), channel->getName(),channel->get_creationtime()),fd);
		return ;
	}
	else if (!channel->get_admin(fd)) {
		sendResponse(ERR_NOTOPERATOR(channel->getName()), fd);
		return ;
	}
	size_t pos = 0;
	for (size_t i = 0; i < modeset.size(); i++) {
		if (modeset[i] == '+' || modeset[i] == '-')
			opera = modeset[i];
		else {
			if (modeset[i] == 'i')
				mode_chain << invite_only(channel, opera, mode_chain.str());
			else if (modeset[i] == 't')
				mode_chain << topic_restriction(channel, opera, mode_chain.str());
			else if (modeset[i] == 'k')
				mode_chain << password_mode(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
			else if (modeset[i] == 'o')
				mode_chain << operator_privilege(tokens, channel, pos, fd, opera, mode_chain.str(), arguments);
			else if (modeset[i] == 'l')
				mode_chain << channel_limit(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
			else
				sendResponse(ERR_UNKNOWNMODE(cli->getNickName(), channel->getName(), modeset[i]), fd);
		}
	}
	std::string chain = mode_chain.str();
	if (!chain.empty())
		channel->sendTo_all(RPL_CHANGEMODE(cli->getHostname(), channel->getName(), chain, arguments));
}


std::string Server::invite_only(Channel *channel, char opera, std::string chain)
{
	std::string param;
	if(opera == '+' && !channel->getModeAtindex(0)) {
		channel->setModeAtindex(0, true);
		channel->setInvitOnly(1);
		param =  mode_toAppend(chain, opera, 'i');
	}
	else if (opera == '-' && channel->getModeAtindex(0)) {
		channel->setModeAtindex(0, false);
		channel->setInvitOnly(0);
		param =  mode_toAppend(chain, opera, 'i');
	}
	return param;
}

std::string Server::topic_restriction(Channel *channel ,char opera, std::string chain)
{
	std::string param;
	if(opera == '+' && !channel->getModeAtindex(1)) {
		channel->setModeAtindex(1, true);
		channel->set_topicRestriction(true);
		param =  mode_toAppend(chain, opera, 't');
	}
	else if (opera == '-' && channel->getModeAtindex(1)) {
		channel->setModeAtindex(1, false);
		channel->set_topicRestriction(false);
		param =  mode_toAppend(chain, opera, 't');
	}	
	return param;
}

bool validPassword(std::string password)
{
	if(password.empty())
		return false;
	for(size_t i = 0; i < password.size(); i++)
	{
		if(!std::isalnum(password[i]) && password[i] != '_')
			return false;
	}
	return true;
}
std::string Server::password_mode(std::vector<std::string> tokens, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments)
{
	std::string param;
	std::string pass;

	if(tokens.size() > pos)
		pass = tokens[pos++];
	else
	{
		sendResponse(ERR_NEEDMODEPARM(channel->getName(),std::string("(k)")),fd);
		return param;
	}
	if(!validPassword(pass))
	{
		sendResponse(ERR_INVALIDMODEPARM(channel->getName(),std::string("(k)")),fd);
		return param;
	}
	if(opera == '+')
	{
		channel->setModeAtindex(2, true);
		channel->setPassword(pass);
		if(!arguments.empty())
			arguments += " ";
		arguments += pass;
		param = mode_toAppend(chain,opera, 'k');
	}
	else if (opera == '-' && channel->getModeAtindex(2)) {
		if(pass == channel->GetPassword()) {		
			channel->setModeAtindex(2, false);
			channel->setPassword("");
			param = mode_toAppend(chain,opera, 'k');
		}
		else
			sendResponse(ERR_KEYSET(channel->getName()),fd);
	}
	return param;
}

std::string Server::operator_privilege(std::vector<std::string> tokens, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments)
{
	std::string user;
	std::string param;

	if(tokens.size() > pos)
		user = tokens[pos++];
	else
	{
		sendResponse(ERR_NEEDMODEPARM(channel->getName(),"(o)"),fd);
		return param;
	}
	if(!channel->clientInChannel(user)) {
		sendResponse(ERR_NOSUCHNICK(channel->getName(), user),fd);
		return param;
	}
	if(opera == '+') {
		channel->setModeAtindex(3,true);
		if(channel->change_clientToAdmin(user))
		{
			param = mode_toAppend(chain, opera, 'o');
			if(!arguments.empty())
				arguments += " ";
			arguments += user;
		}
	}
	else if (opera == '-') {
		channel->setModeAtindex(3,false);
		if(channel->change_adminToClient(user))
		{
			param = mode_toAppend(chain, opera, 'o');
				if(!arguments.empty())
					arguments += " ";
			arguments += user;

		}
	}
	return param;
}


bool Server::isvalid_limit(std::string& limit)
{
	return (!(limit.find_first_not_of("0123456789")!= std::string::npos) && std::atoi(limit.c_str()) > 0);
}

std::string Server::channel_limit(std::vector<std::string> tokens,  Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments)
{
	std::string limit;
	std::string param;
	
	if (opera == '+') {
		if(tokens.size() > pos) {
			limit = tokens[pos++];
			if(!isvalid_limit(limit))
				sendResponse(ERR_INVALIDMODEPARM(channel->getName(),"(l)"), fd);
			else {
				channel->setModeAtindex(4, true);
				channel->setLimit(std::atoi(limit.c_str()));
				if(!arguments.empty())
					arguments += " ";
				arguments += limit;
				param =  mode_toAppend(chain, opera, 'l');
			}
		}
		else
			sendResponse(ERR_NEEDMODEPARM(channel->getName(),"(l)"),fd);
	}
	else if (opera == '-' && channel->getModeAtindex(4)) {
		channel->setModeAtindex(4, false);
		channel->setLimit(0);
		param  = mode_toAppend(chain, opera, 'l');
	}
	return param;
}
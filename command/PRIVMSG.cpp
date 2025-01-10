/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 02:47:17 by danevans          #+#    #+#             */
/*   Updated: 2025/01/07 10:24:32 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int	splitPrivMsg_utils(std::vector<std::pair<std::string, std::string> > &token, std::vector<std::string> splited_cmd)
{
	std::vector<std::string> channels;
	std::vector<std::string> individuals;
	std::string message;
	bool message_found = false;

	if (splited_cmd.size() < 2)
        return (-1);
    for (size_t i = 1; i < splited_cmd.size(); ++i) {
        std::string part = splited_cmd[i];

		if (part == ":" || message_found || part[0] == ':') {
			message_found = true;
			if (!message.empty())
			    message += " ";
			if (part == ":") {
				continue;
			} else if (part[0] == ':' && message.empty()) {
			    message += part.substr(1);
			} else {
			    message += part;
			}
			continue;
		}
        if (part[0] == '#' || part[0] == '&') {
            if (part[part.size() - 1] == ',') 
                part = part.substr(0, part.size() - 1);
            channels.push_back(part);
        } else {
            individuals.push_back(part);
        }
    }
	if (message.empty() || (channels.empty() && individuals.empty()))
		return (0);
	for (size_t i = 0; i < channels.size(); ++i) {
		token.push_back(std::make_pair(channels[i], message));
	}
	for (size_t i = 0; i < individuals.size(); ++i) {
		token.push_back(std::make_pair(individuals[i], message));
	}
	return (1);
}

// std::string composeMessage(const std::string &command, const std::string &message,
// 							const std::string &nick, const std::string &target) {
// 	std::string temp;

// 	temp = ":" + nick + "!" + nick + "@localhost " + command + " " + target + " :" + message + "\n";
// 	// temp = ":" + client->getNickName() + "!~" + client->getUserName() + "@localhost " + command + " " + target + " :" + message + "\r\n";
// 	return (temp);
// }

std::string Server::composeMessage(int fd, const std::string &command, const std::string &message, const std::string &target) {
    std::string temp;
    Client* client = getClient(fd); // Use getClient to retrieve the Client object

    if (client) {
        temp = ":" + client->getNickName() + "!~" + client->getUserName() + "@localhost " + command + " " + target + " :" + message + "\r\n";
    } else {
        // Handle the case where the client is not found
		temp = ":unknown!~unknown@localhost " + command + " " + target + " :" + message + "\r\n";
    }

    return temp;
}

int	Server::PRIVMSG(std::vector<std::string> splited_cmd, Client *client) {
	std::vector<std::pair<std::string, std::string> > token;
	std::string tmp;
	Client *newClient;

	if (!splitPrivMsg_utils(token, splited_cmd)) {
		sendResponse(ERR_NEEDMOREPARAMS(splited_cmd[0]), client->getFd());
		return (0);
	}
	for (size_t i = 0; i < token.size(); ++i) {
		tmp = token[i].first;
		// std::string message = composeMessage(splited_cmd[0], token[i].second,
		// 								client->getNickName(), tmp);
		std::string message = composeMessage(client->getFd(), splited_cmd[0], token[i].second, tmp);
		if (tmp[i] == '#') {
			Channel	*channel;
			channel = getChannel(tmp);
			if (client) {
				//for some reasons unkown admin or owner fd is not in the channel ?
				if (channel->checkClientExistence(client->getFd())) {
					channel->sendTo_all(message, client->getFd());
				}
			}
		} else {
			newClient = getClientNick(tmp);
			if (newClient) {
				sendResponse(message, newClient->getFd());
			} else {
				sendResponse(ERR_NOSUCHNICK(client->getNickName(), tmp), client->getFd());	
			}
		}
	}
	return (1);
}
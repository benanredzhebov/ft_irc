/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 12:58:48 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/23 15:21:58 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*It parses the JOIN command to extract channel names and passwords
RFC 2812 JOIN <channel>{,<channel>} [<key>{,<key>}] */
int Server::splitJoin(std::vector<std::pair<std::string, std::string>> &token, std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string chStr, passStr, buff;
	std::istringstream iss(cmd);

	// Split command into tokens and store in the tmp vector
	while (iss >> cmd)
		tmp.push_back(cmd);
	if (tmp.size() < 2)
	{
		token.clear();
		return 0;
	}
	// extract channel string and password string from the tmp vector
	tmp.erase(tmp.begin()); // deletes the command name JOIN
	chStr = tmp[0];
	tmp.erase(tmp.begin()); // deletes next token after JOIN and remains the passwords
	if (!tmp.empty())
	{
		passStr = tmp[0];
		tmp.clear();
	}

	// Splits the channel string by commas ans stores each channel name in the token vector as a pair with an empty password
	for (size_t i = 0; i < chStr.size(); i++)
	{
		if (chStr[i] == ',')
		{
			token.push_back(std::make_pair(buff, ""));
			buff.clear();
		}
		else
			buff += chStr[i];
	}
	token.push_back(std::make_pair(buff, ""));

	// Splits the password string by commas and assings each password to the corresponding channel in the token vector
	if (!passStr.empty())
	{
		size_t j = 0;
		buff.clear();
		for (size_t i = 0; i < passStr.size(); i++)
		{
			if (passStr[i] == ',')
			{
				token[j].second = buff;
				j++;
				buff.clear();
			}
			else
				buff += passStr[i];
		}
		token[j].second = buff;
	}
	for (size_t i = 0; i < token.size(); i++) // erase the empty channel names
	{
		if (token[i].first.empty())
			token.erase(token.begin() + i--);
	}
	for (size_t i = 0; i < token.size(); i++)
	{ // ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
		if (*(token[i].first.begin()) != '#')
		{
			senderror(403, getClient(fd)->getNickName(), token[i].first, getClient(fd)->getFd(), " :No such channel\r\n");
			token.erase(token.begin() + i--);
		}
		else
			token[i].first.erase(token[i].first.begin());
	}
	return 1;
}

int Server::searchForClients(std::string nickname)
{
	int count = 0;
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getClientInChannel(nickname))
			count++;
	}
	return count;
}
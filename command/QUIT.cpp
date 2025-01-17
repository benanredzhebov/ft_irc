/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:08:19 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/15 10:17:52 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void	Server::sendQuitMessage(Client *client, const std::string& message) {
	std::string trimmedMessage = message;
	if (!trimmedMessage.empty() && trimmedMessage[0] == ':') {
		trimmedMessage = trimmedMessage.substr(1);
	}
	std::string	quitMessage = ":" + client->getNickName() + " QUIT :" + trimmedMessage + "\r\n";
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i].getFd() != client->getFd()) {
			sendResponse(quitMessage, _clients[i].getFd());
		}
	}
}

void	Server::QUIT(Client *client, const std::vector<std::string>& splited_cmd) {
	std::string	quitMessage = "Client quit";
	if (splited_cmd.size() > 1) {
		quitMessage = concatenateVector(splited_cmd.begin() + 1, splited_cmd.end());
	}
	std::cout << RED << "Client fd [" << client->getFd() << "] quit " << quitMessage << RESET << std::endl;
	sendQuitMessage(client, quitMessage);
	removeClientfromChannel(client);
	removeClientInstance(client->getFd());
}
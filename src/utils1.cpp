/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 21:30:29 by danevans          #+#    #+#             */
/*   Updated: 2025/01/17 13:51:44 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <map>

/*GETTERS*/

/*Consider a chat server where each connected client is represented by a Client object.
The server needs to find the Client object associated with a specific file descriptor to
send messages or perform other operations.*/

std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == std::string::npos)
		return ("");
	size_t last = str.find_last_not_of(" \t\n\r");
	return (str.substr(first, (last - first + 1)));
}

std::string toUpper(const std::string& str) {
	std::string result = str;
	for (std::size_t i = 0; i < result.size(); ++i) {
		result[i] = std::toupper(static_cast<unsigned char>(result[i]));
	}
	return result;
}

std::vector<std::string> spliting_cmd(Server *server, Client *cli) {
	char buffer[1024] = {0};
	size_t bytes;
	std::vector<std::string> vec;
	static std::map<int, std::string>	temp_buffers; // map to store buffers for each client

	while (true) {
		bytes = recv(cli->getFd(), buffer, sizeof(buffer) - 1, 0);
		if (bytes <= 0) {
			return vec;
		}
		buffer[bytes] = '\0';
		if (buffer[0] == '\n')
			continue;
		temp_buffers[cli->getFd()].append(buffer);
		if (temp_buffers[cli->getFd()].find('\n') != std::string::npos)
			break;
	}

	std::istringstream stream(temp_buffers[cli->getFd()]);
	std::string command;
	while (std::getline(stream, command, '\n')) {
		if (!command.empty()) {
			command = trim(command);
			std::istringstream cmd_stream(command);
			std::string token;
			std::vector<std::string> splited_cmd;
			while (cmd_stream >> token) {
				splited_cmd.push_back(token);
			}
			if (!splited_cmd.empty()) {
				splited_cmd[0] = toUpper(splited_cmd[0]);
			}
			vec.push_back(server->concatenateVector(splited_cmd));
		}
	}
	temp_buffers[cli->getFd()].clear();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 21:30:29 by danevans          #+#    #+#             */
/*   Updated: 2025/01/14 12:21:02 by beredzhe         ###   ########.fr       */
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

// std::vector<std::string>	spliting_cmd(Client *cli) {
// 	char						buffer[1024] = {0};
// 	size_t						bytes;
// 	std::vector<std::string>	vec;
// 	std::string					cleaned_buffer;
// 	std::string					temp_buffer;

// 	while (true) {	
// 		bytes = recv(cli->getFd(), buffer, sizeof(buffer) - 1, 0);
// 		if (bytes <= 0) {
// 			return vec;
// 		}
// 		buffer[bytes] = '\0';
// 		if (buffer[0] == '\n')
// 			continue ;
// 		temp_buffer.append(buffer);
// 		if (temp_buffer.find('\n') != std::string::npos)
// 			break ;
// 	}
// 	if (!temp_buffer.empty() && temp_buffer[temp_buffer.size() - 1] == '\n') {
// 		temp_buffer.erase(temp_buffer.size() - 1, 1);
// 	}
// 	cleaned_buffer = trim(std::string(temp_buffer));
// 	std::istringstream	stm(cleaned_buffer);
// 	std::string	token;
// 	while (stm >> token) { 
// 		vec.push_back(token);
// 		token.clear();
// 	}
// 	if (!vec.empty()) {
// 		vec[0] = toUpper(vec[0]);
// 	}
// 	return (vec);
// }

std::vector<std::string> spliting_cmd(Client *cli) {
	Server *server;
	char buffer[1024] = {0};
	size_t bytes;
	std::vector<std::string> vec;
	// std::string temp_buffer;
	static std::map<int, std::string>	temp_buffers; // Use a map to store buffers for each client

	while (true) {
		bytes = recv(cli->getFd(), buffer, sizeof(buffer) - 1, 0);
		if (bytes <= 0) {
			return vec;
		}
		buffer[bytes] = '\0';
		// temp_buffer.append(buffer);
		temp_buffers[cli->getFd()].append(buffer);
		// if (temp_buffer.find('\n') != std::string::npos)
		// 	break;
		if (temp_buffers[cli->getFd()].find('\n') != std::string::npos)
			break;
	}

	// std::istringstream stream(temp_buffer);
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
	// temp_buffer.clear();
	temp_buffers[cli->getFd()].clear();
	return vec;
}
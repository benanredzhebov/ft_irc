/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 21:30:29 by danevans          #+#    #+#             */
/*   Updated: 2025/01/02 02:01:27 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

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

std::vector<std::string>	spliting_cmd(Client *cli) {
	char						buffer[1024] = {0};
	size_t						bytes;
	std::vector<std::string>	vec;
	std::string					cleaned_buffer;
	std::string					temp_buffer;

	while (true) {	
		bytes = recv(cli->getFd(), buffer, sizeof(buffer) - 1, 0);
		if (bytes <= 0) {
			return vec;
		}
		buffer[bytes] = '\0';
		temp_buffer.append(buffer);
		if (temp_buffer.find('\n') != std::string::npos)
			break ;
	}
	if (!temp_buffer.empty() && temp_buffer[temp_buffer.size() - 1] == '\n') {
		temp_buffer.erase(temp_buffer.size() - 1, 1);
	}
	cleaned_buffer = trim(std::string(temp_buffer));
	std::istringstream	stm(cleaned_buffer);
	std::string	token;
	while (stm >> token) { 
		vec.push_back(token);
		token.clear();
	}
	if (!vec.empty()) {
		vec[0] = toUpper(vec[0]);
	}
	return (vec);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 21:30:29 by danevans          #+#    #+#             */
/*   Updated: 2024/12/25 21:34:30 by danevans         ###   ########.fr       */
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
	
	bytes = recv(cli->getFd(), buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0) {
		std::cerr << "\n😕Passwoord failed😕\n" << std::endl;
		return vec;
	}
	if (buffer[bytes - 1] == '\n') {
		buffer[bytes - 1] = '\0';
	}
	cleaned_buffer = trim(std::string(buffer));
	std::istringstream	stm(cleaned_buffer);
	std::string	token;
	while (stm >> token) { // Reads tokens from the stream stm, into the string token using >> operator
		vec.push_back(token); // Adds each token to the vectors
		// token.clear();
	}
	if (!vec.empty()) {
		vec[0] = toUpper(vec[0]);
	}
	return (vec);
}
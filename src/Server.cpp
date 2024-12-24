/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:36:20 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/23 18:18:47 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*GETTERS*/

/*Consider a chat server where each connected client is represented by a Client object.
The server needs to find the Client object associated with a specific file descriptor to
send messages or perform other operations.*/
Client *Server::getClient(int fd) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getFd() == fd)
			return &this->_clients[i];
	}
	return (NULL);
}

/*sends a response (after receiving a request) to a client over a network socket*/
void Server::sendResponse(std::string response, int fd) {
	if (send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;
}

std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == std::string::npos)
		return ("");
	size_t last = str.find_last_not_of(" \t\n\r");
	return (str.substr(first, (last - first + 1)));
}

/*receive password_ from the client and verify the password and return*/
int	Server::recievePasswordVerify(int _client_fdsocket){
	char		buffer[1024] = {0};
	size_t		bytes;
	std::string	cleaned_buffer;
	
	bytes = recv(_client_fdsocket, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		std::cerr << "\n😕Passwoord failed😕\n" << std::endl;
		return (0);
	}
	if (buffer[bytes - 1] == '\n'){
		buffer[bytes - 1] = '\0';
	}
	cleaned_buffer = trim(std::string(buffer));
	if (cleaned_buffer == _password) {
    	sendResponse(G_PASSWORD, _client_fdsocket);
    	return (1);
	}
	sendResponse(ERR_INCORPASS(std::string("*")), _client_fdsocket);
	return (0);
}

std::string	Server::receiveSetNameUname(int _client_fdsocket){
	char	buffer[1024] = {0};
	size_t	bytes;
	
	bytes = recv(_client_fdsocket, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		std::cerr << "\n😕Name failed😕\n" << std::endl;
		return (0);
	}
	if (buffer[bytes - 1] == '\n'){
        buffer[bytes - 1] = '\0';
    }
	return(buffer);
}

/*consider a chat application where users can join different channels.
The server needs to find a specific channel by its name to perform
operations like sending messages or adding users.*/
// Channel *Server::getChannel(std::string name) {
// 	for (size_t i = 0; i < this->_channels.size(); i++) {
// 		if (this->_channels[i].getName() == name)
// 			return &_channels[i];
// 	}
// 	return NULL;
// }

/*PARSING METHODS*/

/*splits a given command string into a vector of strings based on whitespace.*/
std::vector<std::string> Server::split_cmd(std::string &cmd) {
	std::vector<std::string>	vec;
	std::istringstream	stm(cmd);
	std::string	token;
	while (stm >> token) { // Reads tokens from the stream stm, into the string token using >> operator
		vec.push_back(token); // Adds each token to the vectors
		token.clear();
	}
	return vec;
}
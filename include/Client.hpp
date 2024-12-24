/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/24 09:18:29 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Server.hpp"

class Client {
	private:
		int			_fd; // File descriptor for the client's socket
		bool		_isOperator; // Flag indicating if the client is an operator
		bool		_registered; // Flag indicating if the client is registered
		bool		_logedin; // Flag indicating if the client is logged in
		std::string	_username; // Username of the client
		std::string	_nickname; // Nickname of the client
		std::string	_buffer; // Buffer for storing received data
		std::vector<std::string> _channelIsInvite; // List of channels the client is invited to
		
	public:
		Client();
		Client(std::string nickname, std::string username, int fd);
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

	/*GETTERS*/
	int			getFd(); // Get the file descriptor
	bool		getRegistered(); // Check if the client is registered
	bool		getInviteChannel(std::string &chName); // Check if the client is invited to a channel
	std::string	getBuffer(); // Get the buffer
	std::string	getHostname(); // Get the hostname
	bool		getLogedIn(); // Check if the client is logged in
	std::string	getNickName(); // Get the nickname
	std::string	getUserName(); // Get the username
	

	/*SETTERS*/
	void	setFd(int fd); // Set the file descriptor
	void	setNickName(std::string &nickname); // Set the nickname
	void	setUsername(std::string& username);  // Set the username
	void	setRegistered(bool value); // Set the registered flag
	void	setLogedIn(bool value); // Set the logged in flag
	void	setBuffer(std::string received); // Set the buffer
	
	/*METHODS*/
	void	clearBuffer(); // Clear the buffer
	void	addChannelInvite(std::string &chname); // Add a channel invite
	void	rmChannelInvite(std::string &chname); // Remove a channel invite
};

#endif
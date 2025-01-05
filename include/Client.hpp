/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/05 22:43:58 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Server.hpp"
#include "./Client.hpp"

class Channel;

class Client {
	private:
		bool						_usernameState;
		int							_fd; // File descriptor for the client's socket
		int							_passwordTrials; // Number of password trials
		bool						_registered; // Flag indicating if the client is registered
		bool						_logedin; // Flag indicating if the client is logged in
		bool						_isOperator; // Flag indicating if the client is an operator
		bool						_passwordVerified; // Flag indicating if the password was verified
		std::string					_username; // Username of the client
		std::string					_nickname; // Nickname of the client
		std::string					_buffer; // Buffer for storing received data
		std::string					_ipadd; // IP address of the client
		bool						_nickbool; // Flag for nickname-related logic
		
		std::vector<Channel>		_channels; // List of channels the client belongs to
		std::vector<std::string>	_channelIsInvite; // List of channels where the client has an invite
	
	public:
		Client();
		Client(const Client &other);
		Client &operator=(const Client &other);
    	~Client();
	
	bool			getUserstate();
	
	int				getFd();
	bool			getRegistered();
	bool			getLogedIn(); // Check if the client is logged in
	std::string		getNickName(); // Get the nickname
	std::string		getUserName(); // Get the username
	bool			nickGet();
	int				getChannelSize() const;
	std::string		getBuffer(); // Get the buffer
	std::string		getIpAdd();
	std::string		getHostname();
	
	bool			getInviteChannel(std::string &chName);



	/*SETTERS*/
	void			setUserstate(bool value); // Set the nickname

	
	void			setNickName(std::string &nickname); // Set the nickname
	void			nickSet(bool value);
	void			setUsername(std::string &username); // Set the username
	void			setRegistered(bool value); // Set the registered flag
	void			setClientFd(int fd);
	void			setLogedIn(bool value); // Set the logged in flag
	void			setIpAdd(std::string ipadd);
	
	// void			setBuffer(std::string received); // Set the buffer
	// void			setPasswordVerified(bool verified);
	// void			addChannelInvite(std::string &chname); // Add a channel invite



	/* METHODS */
	void			decrementPasswordTrials();
    int 			getPasswordTrials() const;	
	void			rmChannelInvite(std::string &chname);
	void			deleteClientfromChannels();
};

#endif
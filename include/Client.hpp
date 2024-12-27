/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/27 18:52:19 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Server.hpp"
#include "./Client.hpp"

class Channel;

class Client {
	private:
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
    // Client() : _fd(-1), _registered(false), _nickbool(false), _logedin(false), _passwordTrials(3){}

	
	
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
	bool			getPasswordVerified() const;



	/*SETTERS*/
	void			setNickName(std::string &nickname); // Set the nickname
	void			nickSet(bool value);
	void			setUsername(std::string &username); // Set the username
	void			setRegistered(bool value); // Set the registered flag
	void			setClientFd(int fd);
	void			setLogedIn(bool value); // Set the logged in flag

	
	void			setFd(int fd); // Set the file descriptor
	void			setBuffer(std::string received); // Set the buffer
	void			setIpAdd(std::string ipadd);
	void			setPasswordVerified(bool verified);



	/* METHODS */
	void			decrementPasswordTrials();
    int 			getPasswordTrials() const;
	
	
	void			clearBuffer(); // Clear the buffer
	void			addChannelInvite(std::string &chname); // Add a channel invite
	void			rmChannelInvite(std::string &chname); // Remove a channel invite
};

#endif
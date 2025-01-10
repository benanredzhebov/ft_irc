/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/10 10:14:52 by beredzhe         ###   ########.fr       */
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
		int							_fd;
		int							_passwordTrials;
		bool						_registered;
		bool						_logedin;
		bool						_isOperator;
		bool						_passwordVerified;
		bool						_nickbool;
		std::string					_username;
		std::string					_nickname;
		std::string					_buffer;
		std::string					_ipadd;
		std::vector<Channel>		_channels;
	
	public:
		Client();
		Client(const Client &other);
		Client &operator=(const Client &other);
    	~Client();
	
	bool			getUserstate();
	int				getFd();
	bool			getRegistered();
	int				getChannelSize() const;
	bool			getLogedIn();
	bool			nickGet();
	std::string		getNickName();
	std::string		getUserName();
	std::string		getIpAdd() const;
	std::string		getHostname();

	/*SETTERS*/
	void			setUserstate(bool value);
	void			setNickName(std::string &nickname);
	void			nickSet(bool value);
	void			setUsername(std::string &username);
	void			setRegistered(bool value);
	void			setClientFd(int fd);
	void			setLogedIn(bool value);
	void			setIpAdd(const std::string ipadd);

	/* METHODS */
	void			decrementPasswordTrials();
    int 			getPasswordTrials() const;	
	void			rmChannelInvite(std::string &chname);
	void			deleteClientfromChannels();
};

#endif
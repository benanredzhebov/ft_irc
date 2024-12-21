/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/21 09:33:04 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Server.hpp"

class Client{
	private:
		int			_fd;
		bool		_registered;
		bool		_logedin;
		std::string	_username;
		std::string	_nickname;
		
	public:
		Client();
		Client(std::string nickname, std::string username, int fd);
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

	/*GETTERS*/
	int			getFd();
	bool		getRegistered();
	bool		getLogedIn();
	std::string	getNickName();
	std::string	getUserName();
	

	/*SETTERS*/
	void	setNickName(std::string &nickname);
	void	setUsername(std::string& username);
	void	setRegistered(bool value);
	void	setLogedIn(bool value);
};

#endif
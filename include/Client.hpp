/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/20 11:33:00 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Server.hpp"

class Client{
	private:
		int			_fd;
		bool		_registered;
		std::string	_username;
		
	public:
		Client();
		Client(std::string nickname, std::string username, int fd);
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

	/*GETTERS*/
	int			getFd();
	bool		getRegistered();
	std::string	getNickName();
	

	/*SETTERS*/
	void	setRegistered(bool value);
};

#endif
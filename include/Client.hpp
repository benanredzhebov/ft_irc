/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/23 21:55:06 by danevans         ###   ########.fr       */
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
		// Client();
		// Client(std::string nickname, std::string username, int fd);
		// ~Client();
		// Client(const Client &other);
		// Client &operator=(const Client &other);
    Client() : _fd(-1), _registered(false), _logedin(false) {}
    ~Client(){
		std::cout << "calleed" << std::endl;
	}

    // Copy constructor
    Client(const Client& other) 
        : _fd(other._fd),
          _registered(other._registered),
          _logedin(other._logedin),
          _nickname(other._nickname),
          _username(other._username) {}

    // Copy assignment operator
    Client& operator=(const Client& other) {
        if (this != &other) {
            _fd = other._fd;
            _registered = other._registered;
            _logedin = other._logedin;
            _nickname = other._nickname;
            _username = other._username;
        }
        return *this;
    }


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

	void	setClientFd(int fd);
};

#endif
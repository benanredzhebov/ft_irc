/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:02:57 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/25 22:13:51 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Server.hpp"

enum ClientState {
	WAITING_FOR_PASSWORD,
	WAITING_FOR_NICKNAME,
	WAITING_FOR_USERNAME,
	AUTHENTICATED
};

class Client {
	private:
		int			_fd;
		int			_passwordTrials;
		bool		_registered;
		bool		_logedin;
		std::string	_username;
		std::string	_nickname;
    	ClientState	state;
		
	public:
		// Client();
		// Client(std::string nickname, std::string username, int fd);
		// ~Client();
		// Client(const Client &other);
		// Client &operator=(const Client &other);
    Client() : _fd(-1), _registered(false), _logedin(false), _passwordTrials(3), state(WAITING_FOR_PASSWORD) {}
    ~Client(){
		std::cout << "calleed" << std::endl;
	}

	void decrementPasswordTrials() { _passwordTrials--; }
    int getPasswordTrials() const { return _passwordTrials; }
    void setState(ClientState s) { state = s; }
    ClientState getState() const { return state; }

    // Copy constructor
    Client(const Client& other) 
        : _fd(other._fd),
          _registered(other._registered),
          _logedin(other._logedin),
          _nickname(other._nickname),
		  _passwordTrials(other._passwordTrials),
          _username(other._username) {}

    // Copy assignment operator
    Client& operator=(const Client& other) {
        if (this != &other) {
            _fd = other._fd;
            _registered = other._registered;
            _logedin = other._logedin;
            _nickname = other._nickname;
            _username = other._username;
			_passwordTrials = other._passwordTrials;
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
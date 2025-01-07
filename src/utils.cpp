/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:26:59 by danevans          #+#    #+#             */
/*   Updated: 2025/01/07 11:31:41 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void	Server::close_fds(int fd) {
	if (fd >= 0){
		close(fd);
	}
}

epoll_event	Server::initEpollEvant(int poll_mode, int fd) {
	epoll_event	events;
	events.data.fd = fd;
	events.events = poll_mode;
	return (events);
}

void Server::removeClientInstance(int fd) {
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0) == -1) {
 	   std::cerr << "Failed to remove fd " << fd << " from epoll: " << strerror(errno) << std::endl;
	}
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ) {
        if (it->getFd() == fd) {
			close_fds(fd);
            it = _clients.erase(it);
			return ;
        } else {
            ++it;
        }
    }
}

void		Server::sendAllClient(std::string message) {
	for(size_t i = 0; i < _clients.size(); i++) {
		sendResponse(message, _clients[i].getFd());
	}
}

//need to change the duplicate method i have in remove_admin (std::string &nick)
void	Server::removeClientfromChannel(Client *cli) {
	for (int i = 0; i < _channels.size(); i++) {
		if (_channels[i].remove_admin(cli->getFd())){
			continue ;
		}
		_channels[i].remove_client(cli->getFd());
	}
}

int	Server::_setServerSocket() {
	if (!_createBindListen())
		return (close_fds(_server_fdsocket), 0);
	server_event = initEpollEvant(EPOLLIN, _server_fdsocket);
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, _server_fdsocket, &server_event) == -1)
		return (0);
	return (1);
}

int	Server::resizeFds() {
	if (epoll_waitfd == _fds.size()){
		_fds.resize(_fds.size() * 2);
		std::cout << "Resized server_fds to: " << _fds.size() << std::endl;	
		return (1);
	}
	return (0);
}

std::string Server::concatenateVector(const std::vector<std::string> &splited_cmd) {
    std::string result;
    for (size_t i = 0; i < splited_cmd.size(); ++i) {
        if (!result.empty()) {
            result += " ";
        }
        result += splited_cmd[i];
    }
    return result;
}

std::string Server::concatenateVector(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end) {
    std::string result;
	for (std::vector<std::string>::const_iterator it = begin; it != end; ++it) {
        if (!result.empty()) {
            result += " ";
        }
        result += *it;
    }
    return result;
}

void Server::handleClientInput(Client* client) {
	std::vector<std::string>	splited_cmd;
	splited_cmd = spliting_cmd(client);

	std::cout << YEL << "Input: " << concatenateVector(splited_cmd) << RESET << std::endl;

	if (!client)
		return ;
	if (splited_cmd.empty()) {
		std::cout << RED << "Client fd [" << client->getFd() << "] disconnected" << RESET << std::endl;
		removeClientfromChannel(client);
		removeClientInstance(client->getFd());
		return ;
	}
	if(splited_cmd[0] == "PASS")
		clientPasswordVerify(client, splited_cmd);
	else if (splited_cmd[0] == "NICK")
		clientNickName(client, splited_cmd);
	else if (splited_cmd[0] == "USER"){
		clientUserName(client, splited_cmd);
	}
	else if (client->getLogedIn()) {
		if(splited_cmd[0] == "JOIN"){
			JOIN(splited_cmd, client);
		}
		else if(splited_cmd[0] == "PRIVMSG") {
			PRIVMSG(splited_cmd, client);
		}
		else if(splited_cmd[0] == "KICK") {
			std::string temp = concatenateVector(splited_cmd);
			KICK(temp, client->getFd());
		}
		else if(splited_cmd[0] == "INVITE") {
			INVITE(splited_cmd, client);
		}
		else if(splited_cmd[0] == "TOPIC"){
			TOPIC(splited_cmd, client);
		}
		else if(splited_cmd[0] == "MODE"){
			std::string	temp = concatenateVector(splited_cmd);
			MODE(temp, client->getFd());
		}
		else if(splited_cmd[0] == "QUIT") {
			QUIT(client, splited_cmd);
		}
	}
	else {
		sendResponse(ERR_UNKNOWNCOMMAND(splited_cmd[0]), client->getFd());
	}
}

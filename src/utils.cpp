/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:26:59 by danevans          #+#    #+#             */
/*   Updated: 2024/12/29 13:41:53 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void	Server::close_fds(int fd){
	if (fd >= 0)
		close(fd);
}

epoll_event	Server::initEpollEvant(int poll_mode, int fd) {
	epoll_event	events;
	events.data.fd = fd;
	events.events = poll_mode;
	return (events);
}

void	Server::removeClient(int fd) {
	sendResponse(PASSWORD_AUTH_FAILED, fd);
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
    	if (it->getFd() == fd) {
    	    _clients.erase(it);
    	    break;
    	}
	}
	close_fds(fd);
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

std::string concatenateVector(const std::vector<std::string> &splited_cmd) {
    std::string result;
    for (size_t i = 0; i < splited_cmd.size(); ++i) {
        if (!result.empty()) {
            result += " ";
        }
        result += splited_cmd[i];
    }
    return result;
}


void Server::handleClientInput(Client* client) {
	std::vector<std::string>	splited_cmd;
	splited_cmd = spliting_cmd(client);

	if (splited_cmd.empty()) {
		splited_cmd[0] = "*";
		sendResponse(ERR_NEEDMOREPARAMS(splited_cmd[0]), client->getFd());
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
		// else if(splited_cmd[0] == "QUIT") {
		// 	signalHandler();
		// }
		// else if(splited_cmd[0] == "TOPIC"){
			
		// }
		// else if(splited_cmd[0] == "MODE"){
			
		// }
	}
}

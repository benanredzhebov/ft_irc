/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:39:45 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/10 16:10:02 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <cctype>
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>

#include "./Client.hpp"
#include "./Channel.hpp"
#include "./replies.hpp"


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"
#define RESET "\e[0m"
#define DEBUG 0


class Client;
class Channel;

class Server {
	private:
		int									_port;
		int									_server_fdsocket;
		int									epfd;	
		int									epoll_waitfd;
		static bool							_signal;
		std::string							_password;
		epoll_event							client_event;
		epoll_event							server_event;
		std::vector<Channel>				_channels;
		std::vector<Client>					_clients;
		std::vector<struct epoll_event>		_fds;
		
	public:
		Server(int port, const std::string& password);
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		static bool	isBotfull;
		// void			addClient(Client newClient);
		int				resizeFds();
		void			sendAllClient(std::string message);
		Client			*getClient(int fd);
		Client			*getClientNick(std::string nickname);
		Channel			*getChannel(std::string name);
		
		void			removeClientInstance(int fd);
		void			removeClientfromChannel(Client *cli);
		void			sendResponse(std::string response, int fd); 
		void			close_fds(int fd);
		static void		signalHandler(int signum);
		
		/*SERVER */
		int				_createBindListen();
		int				_creatingServerSocketFd();
		int				_serverReservePortandIpBind();
		int				_serverListens();
		int				_run_server();
		int				_serverAcceptIncoming();
		int				_setServerSocket();
		epoll_event		initEpollEvant(int poll_mode, int fd);
		
		
		/*MODE CMD*/
		void		MODE(std::string &cmd, int fd); // Handle MODE command
		std::string invite_only(Channel *channel, char opera, std::string chain); // Handle invite-only mode
		std::string topic_restriction(Channel *channel ,char opera, std::string chain); // Handle topic restriction mode
		std::string password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments); // Handle password mode
		std::string operator_privilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments); // Handle operator privilege mode
		std::string channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments); // Handle channel limit mode	
		bool		isvalid_limit(std::string& limit); // Check if limit is valid
		std::vector<std::string> splitParams(std::string params); // Split parameters
		std::string mode_toAppend(std::string chain, char opera, char mode); // Append mode to chain
		void 		getCmdArgs(std::string cmd,std::string& name, std::string& modeset ,std::string &params); // Get command arguments

		
	
		/*	CMD	*/
		void			QUIT(Client *client, const std::vector<std::string>& splited_cmd);
		void			KICK(std::string cmd, int fd); // Handle KICK command
		int				PRIVMSG(std::vector<std::string> splited_cmd, Client *client);
		std::string 	composeMessage(int fd, const std::string &command, const std::string &message, const std::string &target);
		int				JOIN(std::vector<std::string> splited_cmd, Client *client);
		void			INVITE(std::vector<std::string> splited_cmd, Client *client);
		// void			TOPIC(std::vector<std::string> splited_cmd, Client *client);
		void 			TOPIC(std::string& cmd, int fd);

		/*	CMD_UTILS	*/
		void			sendQuitMessage(Client *client, const std::string& message);
		std::string 	concatenateVector(const std::vector<std::string> &splited_cmd);
		std::string 	concatenateVector(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end);
		int				splitJoin(std::vector<std::pair<std::string, std::string> > &token,
						std::vector<std::string> splited_cmd);
		void			newChannelCreate(std::string chName, std::string chPass, Client *cli);
		void			existCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd);
		int				searchForClients(std::string nickname);
		std::string		splitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);

		/*	AUTHENTIFICATION METHODS	*/
		int				clientPasswordVerify(Client *cli, std::vector<std::string> splited_cmd);
		void			handleClientInput(Client* client);
		int				clientNickName(Client *cli, std::vector<std::string> splited_cmd);
		int				clientUserName(Client *cli, std::vector<std::string> splited_cmd);
		int				set_nickname(std::string cmd, Client *cli);
		int				set_username(std::vector<std::string> splited_cmd, Client *cli);
		int				confirmClientInfo(Client *cli);
		bool			nickNameInUse(std::string &nickname);
		bool			userNameInUse(std::string &username);
		bool			is_validNickname(std::string &nickname);
		bool			is_validUserName(std::string &username);
		
		/**/
		void			processBufferedMessages(Client* client);
		std::vector<std::string> split(const std::string& str, char delimiter) {
			std::vector<std::string> tokens;
			std::string token;
			std::istringstream tokenStream(str);
			while (std::getline(tokenStream, token, delimiter)) {
				tokens.push_back(token);
			}
			return tokens;
		}
};

/*	helper functions used	*/
std::string					toUpper(const std::string& str);
std::string					trim(const std::string& str);
std::string					getColonMessage(int x, std::vector<std::string> str);
std::vector<std::string>	spliting_cmd(Client *cli);

#endif

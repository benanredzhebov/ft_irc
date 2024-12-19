/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:39:45 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/19 17:30:48 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <poll.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>

#include "Client.hpp"
#include "Channel.hpp"
#include "replies.hpp"


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Client;
Class Channel;

class Server {
	private:
		int							_port;
		int							_server_fdsocket;
		static bool					_Signal;
		std::string					_password;
		std::vector<Client>			_clients;
		std::vector<Channel>		_channels;
		std::vector<struct pollfd>	_fds;
		struct sockaddr_in			_add;
		struct sockaddr_in			_cliadd;
		struct pollfd				_new_cli;
	public:
		Server();
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		/*GETTERS*/
		static bool	isBotfull;
		int			getFd();
		int			getPort();
		std::string	getPassword();
		Client		*getClient(int fd);
		Client		*getClientNick(std::string nickname);
		Channel		*getChannel(std::string name);

		/*SETTERS*/
		void	setFd(int server_fdsocket);
		void	setPort(int port);
		void	setPassword(std::string password);
		void	addClient(Client newClient);
		void	addChannel(Channel newChannel);
		void	addFds(pollfd newFd);
		void	set_username(std::string username, int fd);
		void	set_nickname(std::string cmd, int fd);

		/*REMOVE METHODS*/
		void	removeClient(int fd);
		void	removeChannel(std::string name);
		void	removeFds(int fd);
		void	rmChannels(int fd);

		/*SEND METHODS*/
		void	senderror(int code, std::string clientname, int fd, std::string msg);
		void	senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg);
		void	sendResponse(std::string response, int fd);

		/*CLOSE AND SIGNAL METHODS*/
		static void	signalHandler(int signum);
		void		close_fds();

		/*SERVER METHODS*/
		void	init(int port, std::string pass);
		void	accept_new_client();
		void	set_server_socket();
		void	receiveNewData(int fd);

		/*PARSING METHODS*/
		std::vector<std::string>	split_recivedBuffer(std::string str);
		std::vector<std::string>	split_cmd(std::string &str);
		void						parse_exec_cmd(std::string &cmd, int fd);

		/*AUTHENTIFICATION METHODS*/
		bool	byPassForBot(std::string cmd, int fd);
		bool	notRegistered(int fd);
		bool	nickNameInUse(std::string &nickname);
		void	is_validNickname(std::string &nickname);
		void	client_authen(int fd, std::string pass);

		/*JOIN CMD*/
		void	JOIN(std::string cmd, int fd);
		int		splitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd);
		void	existCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd);
		void	notExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd);
		int		searchForClients(std::string nickname);

		/*PART CMD*/
		void	PART(std::string cmd, int fd);
		int		splitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);

		/*KICK CMD*/
		void	KICK(std::string cmd, int fd);
		std::string splitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);

		/*PRIVMSG CMD*/
		void	PRIVMSG(std::string cmd, int fd);
		void	checkForChannels_Clients(std::vector<std::string> &tmp, int fd);

		/*QUIT CMD*/
		void	QUIT(std::string cmd, int fd);

		/*MODE CMD*/
		void		mode_command(std::string &cmd, int fd);
		std::string invite_only(Channel *channel, char opera, std::string chain);
		std::string topic_restriction(Channel *channel ,char opera, std::string chain);
		std::string password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
		std::string operator_privilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments);
		std::string channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
		bool		isvalid_limit(std::string& limit);
		std::string mode_toAppend(std::string chain, char opera, char mode);
		std::vector<std::string> splitParams(std::string params);
		void 		getCmdArgs(std::string cmd,std::string& name, std::string& modeset ,std::string &params);
		
		/*TOPIC CMD*/
		std::string tTopic();
		void 		Topic(std::string &cmd, int &fd);
		void		Invite(std::string &cmd, int &fd);
		std::string	gettopic(std::string& input);
		int			getpos(std::string &cmd);
		
};


#endif

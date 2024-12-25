/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:39:45 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/25 08:46:40 by beredzhe         ###   ########.fr       */
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

#include "./Client.hpp"
#include "./Channel.hpp"
#include "./replies.hpp"


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Client;
class Channel;

class Server {
	private:
		int							_port; // Port number the server listens on
		int							_server_fdsocket; // File descriptor for the server socket
		static bool					_Signal; // Signal flag for handling interrupts
		std::string					_password; // Password for server authentication
		std::vector<Client>			_clients; // List of connected clients
		std::vector<Channel>		_channels; // List of channels
		std::vector<struct pollfd>	_fds; // List of file descriptors for polling
		struct sockaddr_in			_add; // Server address structure
		struct sockaddr_in			_cliadd; // Client address structure
		struct pollfd				_new_cli; // Pollfd structure for a new client
	public:
		Server();
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		/*GETTERS*/
		static bool	isBotfull; // Flag to check if bot is full
		int			getFd(); // Get server file descriptor
		int			getPort(); // Get server port
		std::string	getPassword(); // Get server password
		Client		*getClient(int fd); // Get client by file descriptor
		Client		*getClientNick(std::string nickname); // Get client by nickname
		Channel		*getChannel(std::string name); // Get channel by name

		/*SETTERS*/
		void	setFd(int server_fdsocket); // Set server file descriptor
		void	setPort(int port); // Set server port
		void	setPassword(std::string password); // Set server password
		void	addClient(Client newClient); // Add a new client
		void	addChannel(Channel newChannel); // Add a new channel
		void	addFds(pollfd newFd); // Add a new file descriptor to poll
		void	set_username(std::string &username, int fd); // Set username for a client
		void	set_nickname(std::string cmd, int fd); // Set nickname for a client

		/*REMOVE METHODS*/
		void	removeClient(int fd); // Remove a client by file descriptor
		void	removeChannel(std::string name); // Remove a channel by name
		void	removeFds(int fd); // Remove a file descriptor from poll
		void	rmChannels(int fd); // Remove channels associated with a client

		/*SEND METHODS*/
		void	senderror(int code, std::string clientname, int fd, std::string msg); // Send error message to client
		void	senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg); // Send error message to client in a channel
		void	sendResponse(std::string response, int fd); // Send response to client

		/*CLOSE AND SIGNAL METHODS*/
		static void	signalHandler(int signum); // Handle signals
		void		close_fds();  // Close all file descriptors

		/*SERVER METHODS*/
		void	init(int port, std::string pass); // Initialize server
		void	accept_new_client(); // Accept a new client connection
		void	set_server_socket(); // Set up the server socket
		void	receiveNewData(int fd); // Receive new data from a client
		int		_run_server();

		/*PARSING METHODS*/
		std::vector<std::string>	split_recivedBuffer(std::string str); // Split received buffer into commands
		std::vector<std::string>	split_cmd(std::string &str); // Split command string into tokens
		void						parse_exec_cmd(std::string &cmd, int fd); // Parse and execute command

		/*AUTHENTIFICATION METHODS*/
		bool	byPassForBot(std::string cmd, int fd); // Bypass authentication for bot
		bool	notRegistered(int fd); // Check if client is not registered
		bool	nickNameInUse(std::string &nickname); // Check if nickname is in use
		bool	is_validNickname(std::string &nickname); // Check if nickname is valid
		void	client_authen(int fd, std::string pass); // Authenticate client

		/*JOIN CMD*/
		void	JOIN(std::string cmd, int fd); // Handle JOIN command
		int		splitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd); // Split JOIN command
		void	existCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd); // Handle existing channel
		void	notExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd); // Handle non-existing channel
		int		searchForClients(std::string nickname); // Search for clients by nickname

		/*PART CMD*/
		void	PART(std::string cmd, int fd); // Handle PART command
		int		splitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd); // Split PART command

		/*KICK CMD*/
		void	KICK(std::string cmd, int fd); // Handle KICK command
		std::string splitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd); // Split KICK command

		/*PRIVMSG CMD*/
		void	PRIVMSG(std::string cmd, int fd); // Handle PRIVMSG command
		void	checkForChannels_Clients(std::vector<std::string> &tmp, int fd); // Check for channels and clients

		/*QUIT CMD*/
		void	QUIT(std::string cmd, int fd); // Handle QUIT command

		/*MODE CMD*/
		void		mode_command(std::string &cmd, int fd); // Handle MODE command
		std::string invite_only(Channel *channel, char opera, std::string chain); // Handle invite-only mode
		std::string topic_restriction(Channel *channel ,char opera, std::string chain); // Handle topic restriction mode
		std::string password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments); // Handle password mode
		std::string operator_privilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments); // Handle operator privilege mode
		std::string channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments); // Handle channel limit mode
		bool		isvalid_limit(std::string& limit); // Check if limit is valid
		std::string mode_toAppend(std::string chain, char opera, char mode); // Append mode to chain
		std::vector<std::string> splitParams(std::string params); // Split parameters
		void 		getCmdArgs(std::string cmd,std::string& name, std::string& modeset ,std::string &params); // Get command arguments
		
		/*TOPIC CMD*/
		std::string Topic(); // Get topic
		void 		Topic(std::string &cmd, int &fd); // Set topic
		void		Invite(std::string &cmd, int &fd); // Handle invite command
		std::string	gettopic(std::string& input); // Get topic from input
		int			getpos(std::string &cmd); // Get position from command
		
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:39:45 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/25 13:40:58 by danevans         ###   ########.fr       */
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
#include <poll.h> // would later be deleted as i used epoll
#include <sys/epoll.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>

#include "./Client.hpp"
// #include "./Channel.hpp"
#include "./replies.hpp"


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Client;
// class Channel;

class Server {
	private:
		// std::vector<Channel>		_channels; // List of channels
		// static bool					_Signal; // Signal flag for handling interrupts
		// std::vector<struct pollfd>	_fds; // List of file descriptors for polling
		// struct sockaddr_in			_add; // Server address structure
		// struct sockaddr_in			_cliadd; // Client address structure
		// struct pollfd				_new_cli; // Pollfd structure for a new client
	
	
		int									_port; // Port number the server listens on
		int									_server_fdsocket; // File descriptor for the server socket
		std::string							_password; // Password for server authentication
		std::vector<Client>					_clients; // List of connected clients
		int									epfd;	
		std::vector<struct epoll_event>		_fds;
		struct epoll_event 					server_event_gotten[100];
		int									epoll_waitfd;
		epoll_event							client_event;
		epoll_event							server_event;
	public:
		Server(int port, const std::string& password);
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		int _runServerUtils(int x, int fd);

		/*GETTERS*/
		static bool	isBotfull; // Flag to check if bot is full
		int			getFd(); // Get server file descriptor
		int			getPort(); // Get server port
		std::string	getPassword(); // Get server password

		
		Client		*getClient(int fd); // Get client by file descriptor
		Client		*getClientNick(std::string nickname); // Get client by nickname
		
		
		// Channel		*getChannel(std::string name); // Get channel by name

		/*SETTERS*/
		void	setFd(int server_fdsocket); // Set server file descriptor
		void	addFds(pollfd newFd); // Add a new file descriptor to poll

		
		void	addClient(Client newClient); // Add a new client
		
		
		// void	addChannel(Channel newChannel); // Add a new channel
		
		
		int	set_username(std::string username, int fd); // Set username for a client
		int	set_nickname(std::string cmd, int fd); // Set nickname for a client

		/*REMOVE METHODS*/
		void	removeClient(int fd); // Remove a client by file descriptor
		
		void	removeChannel(std::string name); // Remove a channel by name
		void	removeFds(int fd); // Remove a file descriptor from poll
		void	rmChannels(int fd); // Remove channels associated with a client

		/*SEND_RECEIVE METHODS*/
		void	sendResponse(std::string response, int fd); // Send response to client
		int		recievePasswordVerify(int _client_fdsocket);
		
		void	senderror(int code, std::string clientname, int fd, std::string msg); // Send error message to client
		void	senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg); // Send error message to client in a channel

		/*CLOSE AND SIGNAL METHODS*/
		void		close_fds(int fd);  // Close all file descriptors
		static void	signalHandler(int signum); // Handle signals

		/*SERVER METHODS*/
		// void	save_port_passwd(std::string port, std::string pass); // changed to accept str Initialize server
		int				_createBindListen();
		int				_creatingServerSocketFd();
		int				_serverReservePortandIpBind();
		int				_serverListens();
		int				_run_server();
		int				_serverAcceptIncoming();
		epoll_event		initEpollEvant(int poll_mode, int fd);
		int				_setServerSocket(); // Set up the server socket
		
		// void	accept_new_client(); // Accept a new client connection
		
		
		int				resizeFds();
		
		void	receiveNewData(int fd); // Receive new data from a client





		/*PARSING METHODS*/
		std::vector<std::string>	split_recivedBuffer(std::string str); // Split received buffer into commands
		std::vector<std::string>	split_cmd(std::string &str); // Split command string into tokens
		void						parse_exec_cmd(std::string &cmd, int fd); // Parse and execute command



		/*AUTHENTIFICATION METHODS*/
		bool			nickNameInUse(std::string &nickname); // Check if nickname is in use
		bool			userNameInUse(std::string &username); // Check if nickname is in use
		bool			is_validNickname(std::string &nickname); // Check if nickname is valid
		bool			is_validUserName(std::string &username); // Check if nickname is valid
		int				client_authen(int fd); // Authenticate client password
		int				clientInfoSave(int fd); // Authenticate client username && password
		std::string		receiveSetNameUname(int _client_fdsocket);
		

		
		bool	byPassForBot(std::string cmd, int fd); // Bypass authentication for bot
		bool	notRegistered(int fd); // Check if client is not registered
		
		





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
		
		
		// std::string invite_only(Channel *channel, char opera, std::string chain); // Handle invite-only mode
		// std::string topic_restriction(Channel *channel ,char opera, std::string chain); // Handle topic restriction mode
		// std::string password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments); // Handle password mode
		// std::string operator_privilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments); // Handle operator privilege mode
		// std::string channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments); // Handle channel limit mode
		
		
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

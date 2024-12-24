// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   my_test.cpp                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/12/21 16:09:32 by danevans          #+#    #+#             */
// /*   Updated: 2024/12/23 23:08:24 by danevans         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "./include/Server.hpp"

// // Server::Server() {default;}

// // Server::~Server(){
	
// // }

// // Server::Server(const Server &other){

// // }

// // Server &Server::operator=(const Server &other){
	
// // }

// int main(int ac, char **av) {
// 	if (ac != 3) {
// 		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
// 		return (0);
// 	}
// 	Server server;
// 	std::cout << "---SERVER---" << std::endl;
// 	server.save_port_passwd(av[1], av[2]);
// 	std::cout << "got here\n" << std::endl;
// 	server.run_server();
// }

// Server::Server()
//     : _port(0),
//       _server_fdsocket(-1),
//     //   _Signal(false),
//       _password(""),
//       epfd(-1),
//     //   server_event(nullptr),
//       epoll_waitfd(0) {
//     // Initialize address structures to zero
//     memset(&_add, 0, sizeof(_add));
//     memset(&_cliadd, 0, sizeof(_cliadd));
//     memset(&server_event_gotten, 0, sizeof(server_event_gotten));

//     std::cout << "Server object constructed." << std::endl;
// }

// Server::~Server() {
//     // Clean up resources
//     if (_server_fdsocket >= 0) {
//         close(_server_fdsocket);
//     }
//     if (epfd >= 0) {
//         close(epfd);
//     }
//     delete server_event;

//     std::cout << "Server object destructed." << std::endl;
// }

// Server::Server(const Server &other)
//     : _port(other._port),
//       _server_fdsocket(other._server_fdsocket),
//     //   _Signal(other._Signal),
//       _password(other._password),
//       _clients(other._clients),
//     //   _channels(other._channels),
//       _fds(other._fds),
//       _add(other._add),
//       _cliadd(other._cliadd),
//       _new_cli(other._new_cli),
//       epfd(other.epfd),
//       epoll_waitfd(other.epoll_waitfd) {
//     // Deep copy for dynamically allocated members
//     if (other.server_event) {
//         server_event = new epoll_event(*other.server_event);
//     // } else {
//     //     server_event = nullptr;
//     }
//     memcpy(server_event_gotten, other.server_event_gotten, sizeof(server_event_gotten));

//     std::cout << "Server object copied." << std::endl;
// }

// Server &Server::operator=(const Server &other) {
//     if (this != &other) {
//         // Clean up existing resources
//         if (_server_fdsocket >= 0) {
//             close(_server_fdsocket);
//         }
//         if (epfd >= 0) {
//             close(epfd);
//         }
//         delete server_event;

//         // Copy non-dynamic members
//         _port = other._port;
//         _server_fdsocket = other._server_fdsocket;
//         // _Signal = other._Signal;
//         _password = other._password;
//         _clients = other._clients;
//         // _channels = other._channels;
//         _fds = other._fds;
//         _add = other._add;
//         _cliadd = other._cliadd;
//         _new_cli = other._new_cli;
//         epfd = other.epfd;
//         epoll_waitfd = other.epoll_waitfd;

//         // Deep copy for dynamically allocated members
//         if (other.server_event) {
//             server_event = new epoll_event(*other.server_event);
//         // } else {
//         //     server_event = nullptr;
//         }
//         memcpy(server_event_gotten, other.server_event_gotten, sizeof(server_event_gotten));

//         std::cout << "Server object assigned." << std::endl;
//     }
//     return *this;
// }


// void	Server::close_fds(int fd){
// 	if (fd >= 0)
// 		close(fd);
// }

// void	Server::setPort(int port){
// 	_port = port;
// }

// void	Server::setPassword(std::string password){
// 	_password = password;
// }

// void	Server::save_port_passwd(std::string port, std::string pass){
// 	setPort(std::atoi(port.c_str()));
// 	setPassword(pass);
// }

// int Server::_creatingServerSocketFd(){
// 	_server_fdsocket = socket(AF_INET6, SOCK_STREAM, 0);
// 	if (_server_fdsocket < 0 ){
// 		std::cout << "socket creation faliled\n" << std::endl;
// 		return (0);
// 	}
// 	return (1);
// }

// // int Server::_serverReservePortandIpBind(){
// // 	std::string ip = "::1";
// // 	struct sockaddr_in6 address;
// // 	memset(&address, 0, sizeof(address));
// // 	address.sin6_family = AF_INET6;
// // 	std::cout << "port = " << _port << std::endl;
// // 	address.sin6_port = htons(_port);
// // 	inet_pton(AF_INET, ip.c_str(), &address.sin6_addr);
// // 	if (bind(_server_fdsocket, (const struct sockaddr *)&address, sizeof(address)) < 0){
// // 		return (std::cout << "binding faliled\n" << std::endl, 0);
// // 	}
// // 	return (1);

// // }

// int Server::_serverReservePortandIpBind() {
//     struct sockaddr_in6 address; // Use sockaddr_in6 for dual-stack
//     memset(&address, 0, sizeof(address));
//     address.sin6_family = AF_INET6;
//     address.sin6_port = htons(_port);
//     address.sin6_addr = in6addr_any; // Bind to both IPv4 and IPv6

//     // Create dual-stack socket by setting IPV6_V6ONLY to 0
//     int opt = 0;
//     if (setsockopt(_server_fdsocket, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) < 0) {
//         std::cerr << "Failed to set IPV6_V6ONLY option: " << strerror(errno) << std::endl;
//         return 0;
//     }

//     if (bind(_server_fdsocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
//         std::cerr << "Binding failed: " << strerror(errno) << std::endl;
//         return 0;
//     }
//     return 1;
// }


// int Server::_serverListens() {
// 	if (listen(_server_fdsocket, SOMAXCONN) < 0) {
// 		std::cout << "lsiten faliled\n" << std::endl;
// 		return (0);
// 	}
// 	return (1);
// }

// int Server::_createBindListen(){
// 	if (!_creatingServerSocketFd())
// 		return (0);
// 	if (!_serverReservePortandIpBind())
// 		return (0);
// 	if (!_serverListens())
// 		return (0);
// 	std::cout << "\ncreating, binding and listening successfully\n" << std::endl;
// 	return (1);
// }

// epoll_event*	Server::initEpollEvant(int poll_mode, int _server_fdsocket) {
// 	epoll_event* events = new epoll_event;
// 	events->data.fd = _server_fdsocket;
// 	events->events = poll_mode;
// 	return events;

// }

// Client *Server::_serverAcceptIncoming() {
//     int clientFd;
//     socklen_t clientAddrLen;
//     struct sockaddr_in6 clientAddr;

//     clientAddrLen = sizeof(clientAddr);
//     clientFd = accept(_server_fdsocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
//     if (clientFd < 0) {
//         std::cerr << "Accepting client failed\n" << std::endl;
//         return nullptr;
//     }

//     // Dynamically allocate a new Client
//     Client *client = new Client();
//     client->setClientFd(clientFd);

//     std::cout << "Accepted client with fd = " << client->getFd() << std::endl;
//     return client;
// }


// // Client *Server::_serverAcceptIncoming(){
// // 	int					clientFd;
// // 	socklen_t			clientAddrLen;
// // 	struct sockaddr_in6	clientAddr;
// // 	Client 				*client;

// // 	clientAddrLen = sizeof(clientAddr);
// // 	clientFd = accept(_server_fdsocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
// // 	if (clientFd  < 0) {
// // 		std::cerr << "accepting client faliled\n" << std::endl;
// // 		close_fds(clientFd);
// // 		client->setClientFd(-1);
// // 		return (client);
// // 	}
// // 	client->setClientFd(clientFd);
// // 	std::cout << "accepted and now returing fd = " << client->getFd() << std::endl;
// // 	return (client);
// // }

// int Server::run_server() {
// 	epfd = epoll_create1(0);
// 	epoll_event *client_event;
	
// 	if (!_createBindListen())
// 		return (close_fds(_server_fdsocket), 0);
// 	server_event = initEpollEvant(EPOLLIN, _server_fdsocket);
// 	if (epoll_ctl(epfd, EPOLL_CTL_ADD, _server_fdsocket, server_event) == -1)
// 		return (0);
// 	while (1) {
// 		epoll_waitfd = epoll_wait(epfd, server_event_gotten, 100, -1);
// 		if (epoll_waitfd < 0)
// 			return (0);
// 		for (int i = 0; i < epoll_waitfd; i++) {
// 			if (server_event_gotten[i].data.fd == _server_fdsocket) {
// 				Client *client = _serverAcceptIncoming();
// 				// std::cout << "after returning push accepted and now returing fd = " << client->getFd() << std::endl;
// 				if (client->getFd() < 0){
// 					return (close_fds(_server_fdsocket), 0);
// 				}
// 				_clients.push_back(*client);
// 				if (!client_authen(client->getFd())){
// 					close_fds(client->getFd());
// 					std::cerr << "\nauthentication failed\n" << std::endl;
// 				}
// 				client_event = initEpollEvant(EPOLLIN,client->getFd());
// 				if (epoll_ctl(epfd, EPOLL_CTL_ADD, client->getFd(), client_event) == -1)
// 					return (0);
// 			}
// 			// else {
// 			// 	_receveAndSend(server_event_gotten[i].data.fd, epfd, clients_info);
// 			// }
// 		}
// 	}
// 	close_fds(_server_fdsocket);
// }

// // void Server::sendResponse(std::string response, int fd) {
// //     if (send(fd, response.c_str(), response.size(), 0) == -1)
// //         std::cerr << "Response send() failed" << std::endl;
// // }

// // Client* Server::getClient(int fd) {
// // 	for (size_t i = 0; i < this->_clients.size(); i++) {
// // 		if (this->_clients[i].getFd() == fd)
// // 			return &this->_clients[i];
// // 	}
// // 	return nullptr; // Explicitly handle the case where no client is found
// // }

// // Function to trim whitespace from the ends of a string
// std::string trim(const std::string& str) {
//     size_t first = str.find_first_not_of(" \t\n\r");
//     if (first == std::string::npos) return ""; // All whitespace
//     size_t last = str.find_last_not_of(" \t\n\r");
//     return str.substr(first, (last - first + 1));
// }

// // Use this in your comparison



// int	Server::client_authen(int fd) {
// 	Client *cli = getClient(fd);
// 	std::string userName;
// 	std::string nickName;

// 	if (cli == NULL)
// 		return (0);
// 	if(!cli->getRegistered()) {
// 		sendResponse(E_PASSWORD, fd);
// 		if (!recievePasswordVerify(fd)){
// 			return (0);
// 		}
// 		cli->setRegistered(true);
// 	}
// 	sendResponse(USERNAME, fd);
// 	userName = receiveSetNameUname(fd);
// 	set_nickname(userName, fd);
// 	sendResponse(NICKNAME, fd);
// 	nickName = receiveSetNameUname(fd);
// 	set_username(nickName, fd);
// 	return (1);
// }

// /*NICK COMMAND
// Check if a given nickname is valid*/

// bool	Server::is_validNickname(std::string &nickname) {
// 	if (!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
// 		return false;
// 	for (size_t i = 1; i < nickname.size(); i++) {
// 		if (!std::isalnum(nickname[i]) && nickname[i] != '_')
// 			return false;
// 	}
// 	return true;
// }

// /*checks if a given nickname is already in use by any of the connected clients*/
// bool	Server::nickNameInUse(std::string &nickname) {
// 	for (size_t i = 0; i < this->_clients.size(); i++) {
// 		if (this->_clients[i].getNickName() == nickname)
// 			return true;
// 	}
// 	return false;
// }

// /*Consider a chat application where users need to set or change their nicknames.
// The server needs to handle this request, ensuring the nickname is valid and not already in use.*/
// void	Server::set_nickname(std::string cmd, int fd) {
// 	std::string	inuse;
// 	// cmd = cmd.substr(4);
// 	size_t	pos = cmd.find_last_not_of("\t\v "); // Finds the last non-whitespace character
// 	if (pos < cmd.size()) { // Removes trailing whitespace and any leading colon (:)
// 		cmd = cmd.substr(pos);
// 		if (cmd[0] == ':')
// 			cmd.erase(cmd.begin());
// 	}
// 	Client	*cli = getClient(fd);
// 	if (pos == std::string::npos || cmd.empty()) // Check fot missing parameters
// 		{sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd); return ;}
// 	if (nickNameInUse(cmd) && cli->getNickName() != cmd) { // Check if nickname is in use
// 		inuse = "*";
// 		if (cli->getNickName().empty())
// 			cli->setNickName(inuse);
// 		sendResponse(ERR_NICKINUSE(std::string(cmd)), fd);
// 		return;
// 	}
// 	if (!is_validNickname(cmd)) {
// 		sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
// 		return;
// 	}
// 	else { // Set nickname
// 		if(cli && cli->getRegistered()){
// 			std::string oldnick = cli->getNickName();
// 			cli->setNickName(cmd);
// 			// for(size_t i = 0; i < _channels.size(); i++){
// 			// 	Client *cl = _channels[i].getClientInChannel(oldnick);
// 			// 	if(cl)
// 			// 		cl->setNickName(cmd);
// 			// }
// 			if(!oldnick.empty() && oldnick != cmd)
// 			{
// 				if(oldnick == "*" && !cli->getUserName().empty())
// 				{
// 					cli->setLogedIn(true);
// 					sendResponse(RPL_CONNECTED(cli->getNickName()), fd); //connected to IRC server
// 					sendResponse(RPL_NICKCHANGE(cli->getNickName(),cmd), fd);
// 				}
// 				else
// 					sendResponse(RPL_NICKCHANGE(oldnick,cmd), fd);
// 				return;
// 			}
			
// 		}
// 		else if (cli && !cli->getRegistered())
// 			sendResponse(ERR_NOTREGISTERED(cmd), fd);
// 	}
// 	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*" && !cli->getLogedIn())
// 	{
// 		cli->setLogedIn(true);
// 		sendResponse(RPL_CONNECTED(cli->getNickName()), fd);
// 	}
// }

// /*USER COMMAND
// Set the username after connecting. The server need to handle this request, ensuring
// the username is provided and the client is registered.*/
// void	Server::set_username(std::string cmd, int fd)
// {
// 	std::vector<std::string> splited_cmd = split_cmd(cmd);
	
// 	Client *cli = getClient(fd);
// 	if((cli && splited_cmd.size() < 5)) // Check for the missing parameters
// 		{sendResponse(ERR_NOTENOUGHPARAM(cli->getNickName()), fd); return; }
// 	if(!cli  || !cli->getRegistered()) // Check if client is registered
// 		sendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
// 	else if (cli && !cli->getUserName().empty()) // Check if username is already set
// 		{sendResponse(ERR_ALREADYREGISTERED(cli->getNickName()), fd); return;}
// 	else
// 		cli->setUsername(splited_cmd[1]); // Set username
// 	// Check if client is fully loged in
// 	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*"  && !cli->getLogedIn())
// 	{
// 		cli->setLogedIn(true);
// 		sendResponse(RPL_CONNECTED(cli->getNickName()), fd);
// 	}
// }

// int			Client::getFd() {return this->_fd;}
// bool		Client::getRegistered() {return this->_registered;}
// bool		Client::getLogedIn() {return this->_logedin;}
// std::string Client::getNickName() {return this->_nickname;}
// std::string	Client::getUserName() {return this->_username;}

// /*SETTERS*/
// void		Client::setRegistered(bool value) {_registered = value;}

// void		Client::setNickName(std::string &nickname) {this->_nickname = nickname;}

// void		Client::setUsername(std::string &username) {this->_username = username;}

// void		Client::setLogedIn(bool value) {this->_logedin = value;}

// void		Client::setClientFd(int fd) {_fd = fd;}

// Client *Server::getClient(int fd) {
// 	for (size_t i = 0; i < this->_clients.size(); i++) {
// 		if (this->_clients[i].getFd() == fd)
// 			return &this->_clients[i];
// 	}
// 	return (NULL);
// }

// /*sends a response (after receiving a request) to a client over a network socket*/
// void Server::sendResponse(std::string response, int fd) {
// 	if (send(fd, response.c_str(), response.size(), 0) == -1)
// 		std::cerr << "Response send() failed" << std::endl;
// }

// /*receive password_ from the client and verify the password and return*/
// int	Server::recievePasswordVerify(int _client_fdsocket){
// 	char	buffer[1024] = {0};
// 	size_t	bytes;
	
// 	bytes = recv(_client_fdsocket, buffer, sizeof(buffer) - 1, 0);
// 	if (bytes <= 0)
// 	{
// 		std::cerr << "\n😕Passwoord failed😕\n" << std::endl;
// 		return (0);
// 	}
// 	if (buffer[bytes - 1] == '\n'){
//         buffer[bytes - 1] = '\0';
//     }
// 	std::cout << "pass_recived = '" << buffer << "'\n" << std::endl; 
// 	std::cout << "pass_requested = '" << _password << "'\n" << std::endl; 
// 	std::string cleaned_buffer = trim(std::string(buffer));
// std::cout << "pass_received (trimmed) = '" << cleaned_buffer << "'\n" << std::endl;
// if (cleaned_buffer == _password) {
//     std::cout << "Success man\n" << std::endl;
//     return 1;
// }

// 	sendResponse(ERR_INCORPASS(std::string("*")), _client_fdsocket);
// 	return (0);
// }

// std::string	Server::receiveSetNameUname(int _client_fdsocket){
// 	char	buffer[1024] = {0};
// 	size_t	bytes;
	
// 	bytes = recv(_client_fdsocket, buffer, sizeof(buffer) - 1, 0);
// 	if (bytes <= 0)
// 	{
// 		std::cerr << "\n😕Name failed😕\n" << std::endl;
// 		return (0);
// 	}
// 	if (buffer[bytes - 1] == '\n'){
//         buffer[bytes - 1] = '\0';
//     }
// 	return(buffer);
// }

// /*consider a chat application where users can join different channels.
// The server needs to find a specific channel by its name to perform
// operations like sending messages or adding users.*/
// // Channel *Server::getChannel(std::string name) {
// // 	for (size_t i = 0; i < this->_channels.size(); i++) {
// // 		if (this->_channels[i].getName() == name)
// // 			return &_channels[i];
// // 	}
// // 	return NULL;
// // }

// /*PARSING METHODS*/

// /*splits a given command string into a vector of strings based on whitespace.*/
// std::vector<std::string> Server::split_cmd(std::string &cmd) {
// 	std::vector<std::string>	vec;
// 	std::istringstream	stm(cmd);
// 	std::string	token;
// 	while (stm >> token) { // Reads tokens from the stream stm, into the string token using >> operator
// 		vec.push_back(token); // Adds each token to the vectors
// 		token.clear();
// 	}
// 	return vec;
// }

// int	Server::_setServerSocket() {
// 	if (!_createBindListen())
// 		return (close_fds(_server_fdsocket), 0);
// 	server_event = initEpollEvant(EPOLLIN, _server_fdsocket);
// 	if (epoll_ctl(epfd, EPOLL_CTL_ADD, _server_fdsocket, &server_event) == -1)
// 		return (0);
// 	return (1);
// }

// int	Server::resizeFds() {
// 	if (epoll_waitfd == _fds.size()){
// 		_fds.resize(_fds.size() * 2);
// 		std::cout << "Resized server_fds to: " << _fds.size() << std::endl;	
// 		return (1);
// 	}
// 	return (0);
// }
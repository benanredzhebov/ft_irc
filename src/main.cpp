/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:33:37 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/16 14:32:17 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*ports below 1024 are reserved for well-known services and require root privileges to bind*/
bool isPortValid(const std::string &port)
{
	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos)
		return false;
	int portNum = std::atoi(port.c_str());
	return portNum >= 1024 && portNum <= 65535;
}

int main(int ac, char **av) {
	if (ac != 3) {
		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return 0;
	}
	if (!isPortValid(av[1])) {
		std::cout << "Invalid port" << std::endl;
		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return 0;
	}
	int port = std::atoi(av[1]);
	std::string pass = av[2];

	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);

	Server server(port, pass);
	std::cout << "---SERVER---" << std::endl;
	server._run_server();

	return 0;
}
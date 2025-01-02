/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:33:37 by beredzhe          #+#    #+#             */
/*   Updated: 2025/01/01 21:41:51 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

bool isPortValid(const std::string &port)
{
	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos)
		return false;
	int portNum = std::atoi(port.c_str());
	return portNum >= 1024 && portNum <= 65535;
}

int main(int ac, char **av)
{
	int port;
	std::string pass;

	if (ac != 3)
	{
		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return 0;
	}
	if (!isPortValid(av[1]))
	{
		std::cout << "Invalid port" << std::endl;
		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return 0;
	}
	port = std::atoi(av[1]);
	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);
	pass = av[2];
	Server server(port, pass);
	std::cout << "---SERVER---" << std::endl;
	server._run_server();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:33:37 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/28 07:31:26 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int ac, char **av) {
	int			port;
	std::string	pass;

	if (ac != 3) {
		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return 0;
	}
	// signal(SIGINT, signalHandler);
	// signal(SIGQUIT, signalHandler);
	port = std::atoi(av[1]);
	pass = av[2];
	Server server(port, pass);
	std::cout << "---SERVER---" << std::endl;
	server._run_server();
}
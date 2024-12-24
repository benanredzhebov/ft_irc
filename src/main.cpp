/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:33:37 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/23 22:54:37 by danevans         ###   ########.fr       */
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
	port = std::atoi(av[1]);
	pass = av[2];
	Server server(port, pass);
	std::cout << "---SERVER---" << std::endl;
	server._run_server();
}
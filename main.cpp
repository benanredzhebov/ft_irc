/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beredzhe <beredzhe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:33:37 by beredzhe          #+#    #+#             */
/*   Updated: 2024/12/21 10:18:55 by beredzhe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/Server.hpp"

int main(int ac, char **av) {
	Server ser;
	if (ac != 3) {
		std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return 1;
	}
	std::cout << "---SERVER---" << std::endl;
}
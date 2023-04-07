/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 16:07:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 19:07:17 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

bool serverOpen = true;

static void	handleSignal(int signal)
{
	(void)signal;
	serverOpen = false;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << RED << "Usage: ./ircserv <Port> <Password>"
			<< RESET << std::endl;
		return ARG_NB;
	}
	
	try {
		
		Server	server(argv[1], argv[2]);
		
		std::cout << YELLOW << "Server waiting for connections..." << RESET << std::endl;

		signal(SIGINT, handleSignal);
		server.launch();
	}
	catch (const std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

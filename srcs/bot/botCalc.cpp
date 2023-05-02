/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botCalc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:42:02 by bperriol          #+#    #+#             */
/*   Updated: 2023/05/01 16:16:01 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Calc.hpp"
#include <csignal>

bool botOpen = true;

static void	handleSignal(int signal)
{
	(void)signal;
	botOpen = false;
}

int main(int argc, char **argv)
{
	if (argc != 4) {
		std::cerr << RED << "Usage: ./botCalc <Server name> <Port> <Password>"
			<< RESET << std::endl;
		return EXIT_FAILURE;
	}
	try {
		// Calculator	Calculator(argv[1], argv[2]);
		Calc calc(argv[1], argv[2], argv[3]);
		
		// connect to server
		calc.connectToServer();
		std::cout << GREEN << "Calculator socket connected" << RESET
			<< std::endl << std::endl;
		calc.createUser();
		std::cout << GREEN << "Calculator user created" << RESET
			<< std::endl << std::endl;
		// std::cout << GREEN << "Calculator launched" << RESET
		// 	<< std::endl << std::endl;
		std::cout << YELLOW << "Waiting for server's instructions..."
			<< RESET << std::endl;
		signal(SIGINT, handleSignal);
		calc.joinCalcChannel();
		calc.launch();
	}
	catch (const ABot::ABotException &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

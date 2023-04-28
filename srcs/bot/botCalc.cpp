/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:42:02 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/25 11:13:47 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Calculator.hpp"

bool botOpen = true;

static void	handleSignal(int signal)
{
	(void)signal;
	botOpen = false;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << RED << "Usage: ./botCalc <Port> <Password>"
			<< RESET << std::endl;
		return EXIT_FAILURE;
	}
	try {
		Calculator	Calculator(argv[1], argv[2]);
		std::cout << GREEN << "Calculator launched" << RESET
			<< std::endl << std::endl;
		std::cout << YELLOW << "Waiting for server's instructions..."
			<< RESET << std::endl;
		signal(SIGINT, handleSignal);
		Calculator.launch();
	}
	catch (const Calculator::CalculatorException &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

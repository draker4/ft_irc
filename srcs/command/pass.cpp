/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/10 16:15:26 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

// The PASS command is used to set a ‘connection password’. If set, the password
// must be set before any attempt to register the connection is made.
void pass(Server *server, std::string line, int clientSocket)
{
	Client *user = server->getUser(clientSocket);
	std::sstream msg_to_send;

	if (line.find_first_of(" ") == std::string::npos)
	{
		std::cout << RED;
		if (user->getRegistered())
			std::cout < < < < ERR_NEEDMOREPARAMS << std::endl;
		return;
	}
	if (user->getRegistered())
	{
		std::cout << RED << ERR_ALREADYREGISTERED(user->get) << std::endl;
		return;
	}
	user->setPassword();
	// std::cout << BLUE << "PASS command called" << RESET << std::endl;
}

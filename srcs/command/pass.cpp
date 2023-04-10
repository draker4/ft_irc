/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 18:54:03 by draker            #+#    #+#             */
/*   Updated: 2023/04/10 19:06:39 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

// The PASS command is used to set a ‘connection password’. If set, the password
// must be set before any attempt to register the connection is made.
void pass(Server *server, Message message, int clientSocket)
{
	Client *user = server->getUser(clientSocket);

	if (message.getParameters().empty())
		//send ERR_NEEDMOREPARAMS
	if (user->getRegistered())
		// send ERR_ALREADYREGISTERED
	
	std::string	password;
	size_t		pos = 0;
	
	if (message.getParameters().find_first_of(" ") != )
	//if password not good
		//send ERR_PASSWDMISMATCH
	user->setPassword(message.getParameters());
	// std::cout << BLUE << "PASS command called" << RESET << std::endl;
}

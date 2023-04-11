/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 18:54:03 by draker            #+#    #+#             */
/*   Updated: 2023/04/11 17:07:36 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

/**
 * @brief The PASS command is used to set a ‘connection password’.
 * The password supplied must match the one defined in the server configuration.
 * 	
 * Syntax: PASS <password>
 * 
 * Numeric replies:
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_ALREADYREGISTERED (462)
 * 	ERR_PASSWDMISMATCH (464)
 * 
 * Example :
 *  [CLIENT] /PASS secretpassword
 */
void pass(Client *client, const Message &message, Server *server)
{
	/**
	 * @brief The PASS command is used to set a ‘connection password’.
	 * The password supplied must match the one defined in the server configuration.
	 * 	
	 * Syntax: PASS <password>
	 * 
	 * Numeric replies:
	 * 	ERR_NEEDMOREPARAMS (461)
	 * 	ERR_ALREADYREGISTERED (462)
	 * 	ERR_PASSWDMISMATCH (464)
	 * 
	 * Example :
	 *  [CLIENT] /PASS secretpassword
	 */
	
	// std::cout << BLUE << "PASS command called" << RESET << std::endl;

	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("PASS")), 
			client->getClientSocket());
		return ;
	}
	else if (client->getRegistered()) {
		server->sendClient(ERR_ALREADYREGISTERED(client->getNickname()), 
			client->getClientSocket());
		return ;
	}
	else if (message.getParameters().front() != server->getPassword()) {
		std::cout << message.getParameters().front() << ": et :" << server->getPassword() << std::endl;
		server->sendClient(ERROR_MESSAGE(std::string("Wrong password")), 
			client->getClientSocket());
		client->setDeconnect(true);
		return ;
	}
	else
		client->setPassword(true);
}

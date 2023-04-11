/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 18:54:03 by draker            #+#    #+#             */
/*   Updated: 2023/04/11 15:36:45 by bperriol         ###   ########lyon.fr   */
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
	std::cout << BLUE << "PASS command called" << RESET << std::endl;

	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("PASS")), 
			client->getClientSocket());
		return ;
	}
	else if (message.getParameters().front() != server->getPassword()) {
		server->sendClient(ERROR_MESSAGE(std::string("Wrong password")), 
			client->getClientSocket());
		//kill client;
		return ;
	}
	// if (user != 0)
	// {
	// 	if (params.empty() || params[0].empty()) {
	// 		replyMsg = numericReply(srv, fd, "461",
	// 			ERR_NEEDMOREPARAMS(std::string("PASS")));
	// 	}
	// 	else if (user->getPassword() == true) {
	// 		replyMsg = numericReply(srv, fd, "462", ERR_ALREADYREGISTRED); 
	// 	}
	// 	else if (!user->getPassword() && srv->getPassword()
	// 		== params[0]) {	
	// 		user->setPassword(true);
	// 	}
	// }
	// srv->sendClient(fd, replyMsg);
}

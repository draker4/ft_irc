/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 18:54:03 by draker            #+#    #+#             */
/*   Updated: 2023/04/11 16:38:03 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

// The PASS command is used to set a ‘connection password’. If set, the password
// must be set before any attempt to register the connection is made.
void pass(const int &clientSocket, const Message &message, Server *server)
{
	std::cout << BLUE << "PASS command called" << RESET << std::endl;

	// std::string replyMsg;
	Client *client = server->getUser(clientSocket);

	if (!client)
		return ;
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(std::string("0"), std::string("PASS")), clientSocket);
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

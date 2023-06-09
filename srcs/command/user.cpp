/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/13 18:21:30 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The USER command is used at the beginning of a connection to specify 
 * 		  the username and realname of a new user.
 * 		
 * 	The minimum length of <username> is 1, ie. it MUST NOT be empty. 
 * 	If it is empty, the server SHOULD reject the command with ERR_NEEDMOREPARAMS.
 * 
 *  If a client tries to send the USER command after they have already completed
 *  registration with the server, the ERR_ALREADYREGISTERED reply should be sent
 *  and the attempt should fail.
 * 	
 *  SYNTAX : USER <username> 0 * <realname>
 * 
 * 	Numeric Replies:
 * 		ERR_NEEDMOREPARAMS (461)
 * 		ERR_ALREADYREGISTERED (462)
 * 
 *  Examples:
 * 	[Client] USER marine 0 * :Marine Sanjuan
 * 	=> Username is marine, Realname is Marine Sanjuan
 * 
 * 	[Client] USER msanjuan msanjuan localhost :Marine SANJUAN
 *  => Username is msanjuan, Realname is Marine Sanjuan
 */
void user(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "USER command called" << RESET << std::endl;
	
	if (message.getParameters().empty() || message.getParameters().front() == "0"
		|| message.getParameters().size() < 3 ) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("USER")), 
			client->getClientSocket());
	}
	else if (!client->getPasswordStatus()) {
		server->sendClient(ERROR_MESSAGE(std::string("Usage: PASS, NICK, USER.")),
		client->getClientSocket());
	}
	else if (client->getRegistered()) {
		server->sendClient(ERR_ALREADYREGISTERED(client->getNickName()), 
			client->getClientSocket());
	}
	else {
		client->setUserName(message.getParameters()[0]);
		if (message.getParameters().size() > 3)
			client->setRealName(message.getParameters()[3]);
		if (client->getNickName().size()) {
			client->setRegistered(true);
			server->sendWelcome(client);
		}
	}
}

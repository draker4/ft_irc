/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 18:34:38 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 20:05:08 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The WHO command is used to query a list of users who match the provided mask.
 * The server will answer this command with zero, one or more RPL_WHOREPLY, and end the list with RPL_ENDOFWHO.
 * The mask can be one of the following:
 * 		A channel name, in which case the channel members are listed.
 * 		An exact nickname, in which case a single user is returned.
 * Numeric Replies:
 * 	RPL_WHOREPLY (352)
 * 	RPL_ENDOFWHO (315)
 * 	ERR_NOSUCHSERVER (402)
 * 
 * 	Syntax : WHO <mask>
 */

void who(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "WHO command called" << RESET << std::endl;
	
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("WHO")),
			client->getClientSocket());
	}
	else {
		Channel	*channel = server->getChannel(message.getParameters()[0]);
		
		if (channel) {
			Channel::mapClients	clients = channel->getClients();
			for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
				std::string	flags = "H";
				if (channel->getOperGrade(it->first) >= 1)
					flags.append("*");
				flags.append(it->second.prefix);
				server->sendClient(RPL_WHOREPLY(client->getNickName(), it->second.client->getNickName(),
				it->second.client->getUserName(), it->second.client->getRealName(),
				it->second.client->getInet(), channel->getName(), flags),
				client->getClientSocket());
			}
			server->sendClient(RPL_ENDOFWHO(client->getNickName(),
				message.getParameters()[0]), client->getClientSocket());
		}
	}
}

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

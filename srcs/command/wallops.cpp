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
 * @brief The WALLOPS command is used to send a message to all currently connected users
 * 	who have set the ‘w’ user mode for themselves. The <text> SHOULD be non-empty.
 * 	
 * 	Numeric replies:
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_NOPRIVILEGES (481)
 * 
 * 	Syntax : WALLOPS <text>
 */
void wallops(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "INVITE command called" << RESET << std::endl;
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), "WALLOPS"),
			client->getClientSocket());
	} else if (!client->getModeStatus('o')) {
		server->sendClient(ERR_NOPRIVILEGES(client->getNickName()),
			client->getClientSocket());
	} else {
		std::string text = message.getParameters()[0];
		Server::mapClient clients = server->getClients();
		for (Server::itMapClient it = clients.begin(); it != clients.end(); it++) {
			if (it->second->getModeStatus('w')) {
				server->sendClient(RPL_WALLOPS(client->getNickName(), client->getUserName(),
					client->getInet(), text), it->second->getClientSocket());
			}
		}
	}
}

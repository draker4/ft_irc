/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/27 19:32:22 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The NAMES command is used to view the nicknames joined to a channel.
 *  If the channel name is invalid or the channel does not exist, one RPL_ENDOFNAMES 
 * 	numeric containing the given channel name should be returned.
 * 
 * 	Syntax: NAMES <channel>{,<channel>}
 * 	
 * 	Numeric Replies:
 * 	
 * 	RPL_NAMREPLY (353)
 * 	RPL_ENDOFNAMES (366)
 * 
 * 	Examples:
 * 	[CLIENT] /NAMES #test,#42
 * 	[SERVER] <client> <symbol> #test :<nick1> <nick2>
 * 	
 */

static void	list_all(Client *client, Server *server) {
	Server::vecChannel	channels = server->getChannels();
	
	for (Server::itVecChannel it = channels.begin(); it != channels.end(); it++) {
		
		// list only users in channel not secrets
		if (!(*it)->getModeStatus('s')) {

			// get all clients in the channel
			Channel::mapClients	clients = (*it)->getClients();
			
			for (Channel::itMapClients it_client = clients.begin(); it_client != clients.end(); it_client++) {

				// Invisible users (mode +i) are only shown if the client who did the request
				// is also in the same channel
				if (!(it_client->second.client->getModeStatus('i')
					&& !(*it)->isClientInChannel(client->getNickName())))
					server->sendClient(RPL_NAMREPLY(client->getNickName(),
					(*it)->getSymbol(), (*it)->getName(), 
					it_client->second.prefix + it_client->second.client->getNickName()),
					client->getClientSocket());
			}
		}
		
		server->sendClient(RPL_ENDOFNAMES(client->getNickName(), (*it)->getName()),
			client->getClientSocket());
	}
	
	// add all users that are not in any channel (and visible)
	Server::mapClient	clients = server->getClients();

	for (Server::itMapClient it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->getModeStatus('i') && it->second->getChannels().empty()) {
			server->sendClient(RPL_NAMREPLY(client->getNickName(), "*", "channel", 
				it->second->getNickName()),
				client->getClientSocket());
		}
	}
	
	server->sendClient(RPL_ENDOFNAMES(client->getNickName(), "channel"),
		client->getClientSocket());
}

void names(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "NAMES command called" << RESET << std::endl;
	
	// if no channel
	if (message.getParameters().empty()) {
		list_all(client, server);
		return ;
	}
	
}

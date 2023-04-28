/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/28 11:12:04 by bperriol         ###   ########lyon.fr   */
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

static void	send_list_names(Client *client, Server *server, Channel *channel) {
	
	// list only users in channel not secrets
	if (!(channel->getModeStatus('s') && !channel->isClientInChannel(client->getNickName()))) {
	
		// get all clients in the channel
		Channel::mapClients	clients = channel->getClients();
		
		// list of names
		std::string	list_names = "";
		
		for (Channel::itMapClients it_client = clients.begin(); it_client != clients.end(); it_client++) {

			// Invisible users (mode +i) are only shown if the client who did the request
			// is also in the same channel
			if (!(it_client->second.client->getModeStatus('i')
				&& !channel->isClientInChannel(client->getNickName()))) {
				if (!list_names.empty())
					list_names.append(" ");
				list_names.append(it_client->second.prefix + it_client->second.client->getNickName());
			}
		}
		
		server->sendClient(RPL_NAMREPLY(client->getNickName(),
			channel->getSymbol(), channel->getName(), list_names),
			client->getClientSocket());
	}
	server->sendClient(RPL_ENDOFNAMES(client->getNickName(), channel->getName()),
		client->getClientSocket());
}

static void	list_all(Client *client, Server *server) {
	Server::vecChannel	channels = server->getChannels();
	
	for (Server::itVecChannel it = channels.begin(); it != channels.end(); it++) {
		send_list_names(client, server, *it);
	}
	
	// add all users that are not in any channel (and visible)
	Server::mapClient	clients = server->getClients();
	bool				no_client = true;

	for (Server::itMapClient it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->getModeStatus('i') && it->second->getChannels().empty()) {
			server->sendClient(RPL_NAMREPLY(client->getNickName(), "*", "channel", 
				it->second->getNickName()),
				client->getClientSocket());
			no_client = false;
		}
	}
	
	if (!no_client)
		server->sendClient(RPL_ENDOFNAMES(client->getNickName(), "channel"),
			client->getClientSocket());
}

void names(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "NAMES command called" << RESET << std::endl;
	
	// if no channel or argument is the servername
	if (message.getParameters().empty() || message.getParameters()[0] == SERVERNAME) {
		list_all(client, server);
		return ;
	}
	
	// gte all channels entered by the user iun the command
	Message::vecString	parameters = split(message.getParameters()[0], ",");

	for (Message::itVecString it = parameters.begin(); it != parameters.end(); it++) {
		
		// get channel from server
		Channel	*channel = server->getChannel(*it);
		
		// check if the channel exists
		if (!channel) {
			server->sendClient(RPL_ENDOFNAMES(client->getNickName(), *it),
				client->getClientSocket());
			continue ;
		}
		
		// send list clients' names in channel
		send_list_names(client, server, channel);
	}
}

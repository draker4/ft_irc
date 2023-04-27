/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/27 11:06:44 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The PART command removes the client from the given channel(s).
 * 
 * 	Syntax: PART <channel>{,<channel>} [<reason>]
 * 
 *  On sending a successful PART command, the user will receive a PART message 
 *  from the server for each channel they have been removed from. 
 *  <reason> is the reason that the client has left the channel(s).
 * 
 *  Numeric Replies:
 *   ERR_NEEDMOREPARAMS (461)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_NOTONCHANNEL (442)
 * 	
 * 	Example:
 * 	[IRSSI] /PART #test,#hey :Dining
 * 	[SERVER] leaves both channels #test and #hey with the reason "Dining"
 * 	[SERVER to CLIENT]"@user_id PART #channel Dining" (for EACH channel they leave)
 */
void part(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "PART command called" << RESET << std::endl;
	
	// if no channel
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("PART")), 
			client->getClientSocket());
		return ;
	}

	// get reason for leaving
	std::string	reason = "";
	if (message.getParameters().size() >= 2)
		reason.append(" " + message.getParameters()[1]);
	
	// get all channels this client wants to leave
	Message::vecString	channels = split(message.getParameters()[0], ",");
	
	// leave each channel
	for (Message::itVecString it = channels.begin(); it != channels.end(); it++) {

		// to check if the channel exists
		Channel	*channel = server->getChannel(*it);
		if (!channel) {
			server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(), *it), 
			client->getClientSocket());
			continue ;
		}
		
		// to check if the user is in the channel
		if (!channel->isClientInChannel(client->getNickName())) {
			server->sendClient(ERR_NOTONCHANNEL(client->getNickName(), channel->getName()), 
				client->getClientSocket());
			continue ;
		}
		
		// SUCCESS
		
		// send PART message to all users of the channel
		Channel::mapClients	clients = channel->getClients();
		for (Channel::itMapClients it_client = clients.begin(); it_client != clients.end(); it_client++) {
			server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(), client->getInet(),
				std::string("PART"), *it + reason), it_client->second.client->getClientSocket());
		}

		// remove client from the channel clients' list
		channel->removeClient(client);
		
		// remove channel from client
		client->removeChannel(channel);
	}
}

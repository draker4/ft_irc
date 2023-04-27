/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:17 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The KICK command can be used to request the forced removal of a user 
 *  from a channel. It causes the <user> to be removed from the <channel> by force. 
 *  If no comment is given, the server SHOULD use a default message instead.
 * 
 *  Parameters: <channel> <user> *( "," <user> ) [<comment>]
 * 
 * Numeric Replies:
 * 
 * ERR_NEEDMOREPARAMS (461)
 * ERR_NOSUCHCHANNEL (403)
 * ERR_CHANOPRIVSNEEDED (482)
 * ERR_USERNOTINCHANNEL (441)
 * ERR_NOTONCHANNEL (442)
 * 
 * Example:
 * Client's request : KICK #Finnish John :Speaking English
 * 
 * Server's Response: " Command to kick John from #Finnish using "Speaking English" 
 * 						as the reason (comment)."
 *
 * @param server
 * @param cmd_infos Structure w/ prefix, command name and message
 */
void kick(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "KICK command called" << RESET << std::endl;
	// check if enough parameters
	if (message.getParameters().size() < 2 ) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), "KICK"),
			client->getClientSocket());
		return;
	} else if (server->channelExist(message.getParameters()[0])) { // check if channel exist
		Channel *channel = server->getChannel(message.getParameters()[0]);
		if (!channel->isClientInChannel(client->getNickName())) { // check if user is on channel
			server->sendClient(ERR_NOTONCHANNEL(client->getNickName(), channel->getName()),
				client->getClientSocket());
		} else if (channel->getOperGrade(client->getNickName()) < 2) { // check if user is an half-operator
			server->sendClient(ERR_HALF_CHANOPRIVSNEEDED(client->getNickName(), channel->getName()),
				client->getClientSocket());
		} else {
			std::string reason = "no explanation";
			if (message.getParameters().size() > 2) {
				reason = message.getParameters().back();
			}
			if (channel->isClientInChannel(message.getParameters()[1])) {
				// send info to client kicked
				Client *clientKicked = server->getClient(message.getParameters()[1]);
				server->sendClient(RPL_KICK(client->getNickName(), client->getUserName(), client->getInet(),
					channel->getName(), message.getParameters()[1], reason), clientKicked->getClientSocket());
				channel->removeClient(clientKicked);
				clientKicked->removeChannel(channel);
				// send info to all client on channel
				Channel::mapClients	clients = channel->getClients();
				for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
					server->sendClient(RPL_KICK(client->getNickName(), client->getUserName(), client->getInet(),
					channel->getName(), message.getParameters()[1], reason),
						it->second.client->getClientSocket());
				}
			} else {
				// send error to client that user is not on channel
				server->sendClient(ERR_USERNOTINCHANNEL(client->getNickName(), message.getParameters()[1],
					channel->getName()), client->getClientSocket());
			}
		}
	} else {
		server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(), message.getParameters()[0]),
				client->getClientSocket());
	}
}

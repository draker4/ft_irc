/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:12 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The INVITE command is used to invite a user to a channel. 
 * 	The parameter <nickname> is the nickname of the person to be invited to the 
 * 	target channel <channel>.
 * 	Numeric Replies:
 * 		RPL_INVITING (341)
 * 		ERR_NEEDMOREPARAMS (461)
 * 		ERR_NOSUCHCHANNEL (403)
 * 		ERR_NOTONCHANNEL (442)
 * 		ERR_CHANOPRIVSNEEDED (482)
 * 		RR_USERONCHANNEL (443)
 * 	Syntax : INVITE <nickname> <channel>
 * 
 */
void invite(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "INVITE command called" << RESET << std::endl;
	// check if enough parameters
	if (message.getParameters().size() < 2) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), "INVITE"),
			client->getClientSocket());
		return;
	}
	else if (server->channelExist(message.getParameters()[1]))
	{
		Channel *channel = server->getChannel(message.getParameters()[1]);
		if (!channel->isClientInChannel(client->getNickName())) {// check if user is on channel
			server->sendClient(ERR_NOTONCHANNEL(client->getNickName(), channel->getName()),
				client->getClientSocket());	
		} else if (channel->isClientInChannel(message.getParameters()[0])) { // check if already in the channel
			server->sendClient(ERR_USERONCHANNEL(client->getNickName(), message.getParameters()[0],
				channel->getName()), client->getClientSocket());
		} else if (channel->getOperGrade(client->getNickName()) < 2) // check if user is at least an half-operator
			server->sendClient(ERR_HALF_CHANOPRIVSNEEDED(client->getNickName(), channel->getName()),
				client->getClientSocket());
		else {
			// send confirmation to user
			server->sendClient(RPL_INVITING(client->getNickName(), message.getParameters()[0], 
				channel->getName()), client->getClientSocket());
			// send invite to the invited user
			if (server->isClientInServer(message.getParameters()[0])) { // check if user invited exist
				server->sendClient(RPL_INVITE(client->getNickName(), client->getUserName(), client->getInet(),
					message.getParameters()[1]),server->getClient(message.getParameters()[0])->getClientSocket());
			}
			channel->addInvited(message.getParameters()[0]);
		}
	} else
		server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(), message.getParameters()[1]),
			client->getClientSocket());
}

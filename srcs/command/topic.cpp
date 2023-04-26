/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/26 14:42:38 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief Command : TOPIC <channel> [<topic>]
 * 			
 * 	The TOPIC command is used to change or view the topic of the given channel. 
 * 	If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned 
 * 	specifying the current channel topic or lack of one. 
 * 	If <topic> is an empty string, the topic for the channel will be cleared.
 * 
 * 	Numeric Replies:
 * 	
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_NOSUCHCHANNEL (403)
 * 	ERR_NOTONCHANNEL (442)
 * 	RPL_NOTOPIC (331)
 * 	RPL_TOPIC (332)
 * 	
 * 	Examples:
 * 	[CLIENT] TOPIC #test :New topic
 *  [SERVER] ; Setting the topic on "#test" to "New topic".
 * 
 * 	[CLIENT] TOPIC #test :
 * 	[SERVER] ; Clearing the topic on "#test"
 * 
 * 	[CLIENT] TOPIC #test
 * 	[SERVER] ; Checking the topic for "#test"
 * 
 */

static void	change_topic(Client *client, const Message &message, Server *server, Channel *channel)
{
	channel->setTopic(client->getNickName(), message.getParameters()[1]);
	
	Channel::mapClients	clients = channel->getClients();
	for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
		server->sendClient(RPL_TOPIC(client->getNickName(), channel->getName(),
		channel->getTopic()), it->second.client->getClientSocket());
	}
}

static void	topic_success_command(Client *client, const Message &message, Server *server, Channel *channel)
{
	// show channel topic
	if (message.getParameters().size() < 2) {
		std::string	topic = channel->getTopic();
		
		// no topic
		if (topic.empty())
			server->sendClient(RPL_NOTOPIC(client->getNickName(), channel->getName()), 
					client->getClientSocket());
		
		// show topic
		else {
			server->sendClient(RPL_TOPIC(client->getNickName(), channel->getName(), channel->getTopic()), 
				client->getClientSocket());
			server->sendClient(RPL_TOPICWHOTIME(client->getNickName(), channel->getName(),
				channel->getClientTopic(), channel->getTimeTopic()), client->getClientSocket());
		}
	}

	// to check if the channel required that the topic command is sent by an half-operator or an channel operator
	else if (channel->getModeStatus('t') && channel->getOperGrade(client->getNickName()) < 2 )
		server->sendClient(ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName()), 
			client->getClientSocket());

	// change topic
	else 
		change_topic(client, message, server, channel);
}

static void	topic_if_channel_exists(Client *client, const Message &message, Server *server, Channel *channel)
{
	// to check if the client is in the channel
	if (!channel->isClientInChannel(client->getNickName()))
		server->sendClient(ERR_NOTONCHANNEL(client->getNickName(), channel->getName()), 
			client->getClientSocket());

	// SUCCESS
	else
		topic_success_command(client, message, server, channel);
}

void topic(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "TOPIC command called" << RESET << std::endl;
	
	// if not enough parameters
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("TOPIC")), 
			client->getClientSocket());
		return ;
	}
	
	// to check if the channel exists
	Channel	*channel = server->getChannel(message.getParameters()[0]);
	if (!channel)
		server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(), message.getParameters()[0]), 
			client->getClientSocket());
	else 
		topic_if_channel_exists(client, message, server, channel);
}

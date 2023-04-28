/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/28 13:49:18 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The JOIN command indicates that the client wants to join the given channel(s), 
 * 	each channel using the given key for it. The server receiving the command checks 
 * 	whether or not the client can join the given channel, and processes the request. 
 * 
 * 	While a client is joined to a channel, they receive all relevant information about 
 * 	that channel including the JOIN, PART, KICK, and MODE messages affecting the channel. 
 * 	They receive all PRIVMSG and NOTICE messages sent to the channel, and they also 
 * 	receive QUIT messages from other clients joined to the same channel (to let them 
 * 	know those users have left the channel and the network). 
 * 
 *  Numeric Replies:
 *  
 *  ERR_NEEDMOREPARAMS (461)
 *  ERR_NOSUCHCHANNEL (403)
 *  ERR_TOOMANYCHANNELS (405)
 *  ERR_BADCHANNELKEY (475)
 *  ERR_BANNEDFROMCHAN (474)
 *  ERR_CHANNELISFULL (471)
 *  ERR_INVITEONLYCHAN (473)
 *  ERR_BADCHANMASK (476)
 *  RPL_TOPIC (332)
 *  RPL_TOPICWHOTIME (333)
 *  RPL_NAMREPLY (353)
 *  RPL_ENDOFNAMES (366)
 *  
 *  Examples:
 * 	[CLIENT]  JOIN #foobar
 *  [SERVER] ; join channel #foobar.
 * 
 * 	[CLIENT]  JOIN #foo,#bar fubar,foobar
 * 	[SERVER]; join channel #foo using key "fubar" and channel #bar using key "foobar".
 */

static void	addClient(Server *server, Client *client, Channel *channel)
{
	// check if user is not banned from channel
	if (channel->isBanned(client))
		server->sendClient(ERR_BANNEDFROMCHAN(client->getNickName(), channel->getName()), 
			client->getClientSocket());
	
	// check if channel is not full
	else if (channel->getModeStatus('l') && channel->isFull())
		server->sendClient(ERR_CHANNELISFULL(client->getNickName(), channel->getName()), 
			client->getClientSocket());
	
	// check if user has to be invited and is invited
	else if (channel->getModeStatus('i') && !channel->isInvited(client->getNickName()))
		server->sendClient(ERR_INVITEONLYCHAN(client->getNickName(), channel->getName()), 
			client->getClientSocket());
	
	// SUCCESS
	else {

		client->addChannel(channel);
		channel->addClient(client);
		Channel::mapClients	clients = channel->getClients();
		std::string	list_names = "";

		// send JOIN message to all clients in the channel
		for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
			server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
				client->getInet(), std::string("JOIN"), channel->getName()), 
				it->second.client->getClientSocket());
			if (it != clients.begin())
				list_names.append(" ");
			list_names.append(channel->getPrefix(it->first) + it->second.client->getNickName());
		}

		//send channel topic
		if (!channel->getTopic().empty()) {
			server->sendClient(RPL_TOPIC(client->getNickName(), channel->getName(),
				channel->getTopic()), client->getClientSocket());
			server->sendClient(RPL_TOPICWHOTIME(client->getNickName(), channel->getName(),
				channel->getClientTopic(), channel->getTimeTopic()), client->getClientSocket());
		}
		
		// send list of names in the current channel
		server->sendClient(RPL_NAMREPLY(client->getNickName(), channel->getSymbol(),
					channel->getName(), list_names), client->getClientSocket());
		server->sendClient(RPL_ENDOFNAMES(client->getNickName(), channel->getName()),
			client->getClientSocket());
	}
}

void join(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "JOIN command called" << RESET << std::endl;
	
	// if client is not registered
	if (!client->getModeStatus('r')) {
		if (!client->getNickName().empty())
			server->sendClient(ERR_NONICKNAMEGIVEN(client->getNickName()), 
				client->getClientSocket());
		else
			server->sendClient(ERR_NONICKNAMEGIVEN("*"), 
				client->getClientSocket());
		return ;
	}
	
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("JOIN")), 
			client->getClientSocket());
	}
	else if (message.getParameters()[0] == "0") {
		//PART all channels
	}
	else {
		Message::vecString	channels = split(message.getParameters()[0], ",");

		Message::vecString	keys;
		if (message.getParameters().size() > 1)
			keys = split(message.getParameters()[1], ",");
		Message::itVecString	itKeys = keys.begin();
		
		for (Message::itVecString it = channels.begin(); it != channels.end(); it++) {
			
			// BANCHANMASK

			// check first char of the channel name
			if ((*it)[0] != '#' && (*it)[0] != '&') {
				server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(), *it), 
					client->getClientSocket());
				continue ;
			}
			
			// check how many channels the client already follows
			if (client->getChannels().size() >= CHANLIMIT) {
				server->sendClient(ERR_TOOMANYCHANNELS(client->getNickName(), *it), 
					client->getClientSocket());
				continue ;
			}
			
			// check if the channel exists
			Channel	*channel = server->getChannel(*it);
			if (!channel) {
				
				// create channel
				channel = new Channel(*it, client);
				// Channel::mapClients cli = channel->getClients();
				// std::cout << "HERE begin=" << cli.size() << std::endl;
				// std::cout << "here begin=" << cli.begin()->first << std::endl;
				// std::cout << "here begin=" << (++cli.begin())->first << std::endl;
				
				// add channel to server and to client's channel list
				server->addChannel(channel);
				client->addChannel(channel);
				server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
					client->getInet(), std::string("JOIN"), channel->getName()), 
					client->getClientSocket());
				server->sendClient(RPL_NAMREPLY(client->getNickName(), channel->getSymbol(),
					channel->getName(), channel->getPrefix(client->getNickName()) + client->getNickName()),
					client->getClientSocket());
				server->sendClient(RPL_ENDOFNAMES(client->getNickName(), channel->getName()),
					client->getClientSocket());
			}
			else if (!channel->isClientInChannel(client->getNickName())) {

				// check if channel need a key
				if (channel->getModeStatus('k')) {
					if (itKeys != keys.end() && channel->getKey() == (*itKeys).substr(0, 10))
						addClient(server, client, channel);
					else
						server->sendClient(ERR_BADCHANNELKEY(client->getNickName(), *it), 
							client->getClientSocket());
				}
				else
					addClient(server, client, channel);
			}
			if (itKeys != keys.end())
				itKeys++;
		}
	}
}

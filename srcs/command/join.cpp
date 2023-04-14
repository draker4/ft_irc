/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/14 15:28:23 by bperriol         ###   ########lyon.fr   */
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

typedef std::vector<std::string>			vecString;
typedef std::vector<std::string>::iterator	itVecString;
typedef	std::string::iterator				itString;

static vecString	split(std::string str, std::string c)
{
	vecString	vec;
	size_t		prev = 0;
	size_t		pos;

	while ((pos = str.find_first_of(c, prev)) != std::string::npos) {
		vec.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	if (str[prev])
		vec.push_back(str.substr(prev, str.length() - prev));
	return vec;
}

static void	addClient(Client *client, Channel *channel)
{

}

void join(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "JOIN command called" << RESET << std::endl;
	
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("JOIN")), 
			client->getClientSocket());
	}
	if (message.getParameters()[0] == "0") {
		//PART all channels
	}
	else {
		vecString	channels = split(message.getParameters()[0], ",");
		
		for (itVecString it = channels.begin(); it != channels.end(); it++) {
			std::cout << UNDERLINE << PURPLE << *it << RESET << std::endl;
		}
		
		vecString	keys;
		if (message.getParameters().size() > 1)
			keys = split(message.getParameters()[1], ",");
		itVecString	itKeys = keys.begin();
		
		for (itVecString it = keys.begin(); it != keys.end(); it++) {
			std::cout << UNDERLINE << GREEN << *it << RESET << std::endl;
		}
		
		for (itVecString it = channels.begin(); it != channels.end(); it++) {
			
			// check first char of the channel name
			if ((*it)[0] == '#' || (*it)[0] == '&')
				server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(), *it), 
					client->getClientSocket());
			
			// check how many channels the client already follows
			if (client->getChannels().size() >= CHANLIMIT)
				server->sendClient(ERR_TOOMANYCHANNELS(client->getNickName(), *it), 
					client->getClientSocket());
			
			// check if the channel exists
			
			Channel	*channel = server->getChannel(*it);
			if (!channel) {
				
				// create channel
				channel = new Channel(*it, client);
				
				// add channel to server and to client's channel list
				server->addChannel(channel);
				client->addChannel(channel);
				
			} else {
				
				// check if channel need a key
				if (channel->getModeStatus('k')) {
					if (channel->getKey() == *itKeys)
						addClient(client,channel);
					else
						server->sendClient(ERR_BADCHANNELKEY(client->getNickName(), *it), 
							client->getClientSocket());
				}
				else
					addClient(client,channel);
			}
			itKeys++;
		}
	}
}

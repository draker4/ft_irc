/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 18:34:38 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 12:41:13 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The WHO command is used to query a list of users who match the provided mask.
 * The server will answer this command with zero, one or more RPL_WHOREPLY, and end the list with RPL_ENDOFWHO.
 * The mask can be one of the following:
 * 		A channel name, in which case the channel members are listed.
 * 		An exact nickname, in which case a single user is returned.
 * Numeric Replies:
 * 	RPL_WHOREPLY (352)
 * 	RPL_ENDOFWHO (315)
 * 	ERR_NOSUCHSERVER (402)
 * 
 * 	Syntax : WHO <mask>
 */

void arg_channel(Client *client, const Message &message, Server *server)
{
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

static void	who_all(Client *client, Server *server) {

	// get all clients in the server
	Server::mapClient	clients = server->getClients();
	
	for (Server::itMapClient it = clients.begin(); it != clients.end(); it++) {

		// list only visible clients or clients that share a common channel
		if (!(it->second->getModeStatus('i') && !it->second->shareChannel(client->getChannels()))) {
			
			//get arbitrary channel name and flags
			Client::vecChannel	channels = it->second->getChannels();
			
			std::string	channel_name = "";
			std::string	flags = "H";
			if (channels[0]->getOperGrade(it->second->getNickName()) >= 1)
				flags.append("*");
			// flags.append(channels[0]->getClientPrefix(it->second));
			
			if (channels.empty())
				channel_name = "*";
			else
				channel_name = channels[0]->getName();

			// list client
			// server->sendClient(RPL_WHOREPLY(client->getNickName(), it->second.client->getNickName(),
			// it->second.client->getUserName(), it->second.client->getRealName(),
			// it->second.client->getInet(), channel_name, flags),
			// client->getClientSocket());
		}
	}
}

void who(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "WHO command called" << RESET << std::endl;

	// bool	list_ops_only = false;

	// if no arg
	// list all users (visible or share a common channel)
	if (message.getParameters().empty()) {
		who_all(client, server);
	}
	
	// if second arg == 'o', list only operators checking the mask given
	if (message.getParameters().size() >= 2
		&& message.getParameters()[1] == "o") {
		// list_ops_only = true;
	}
	
	// if first arg a channel
	
	// if first arg not a channel, list all users checking the mask with
	// their nickname, username, realname, host, server
	
	// send END of WHO reply
}

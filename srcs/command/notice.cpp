/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/25 11:21:51 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The NOTICE command is used to send notices between users, as well as to send notices to channels. 
 * <target> is interpreted the same way as it is for the PRIVMSG command.
 * 
 * The NOTICE message is used similarly to PRIVMSG. 
 * The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message. 
 * This rule also applies to servers – they must not send any error back to the client on receipt of a NOTICE command. 
   @param server
   @param client_fd User sending a msg
   @param cmd_infos Structure w/ prefix, command name and message
   Useful link : https://modern.ircdocs.horse/#notice-message
 * 
 */

static bool	is_target_channel(char c, std::string str)
{
	size_t	pos = str.find_first_of(c);
	if (pos != std::string::npos)
		return true;
	return false;
}

static Server::vecClient	clients_from_channel(Client *client, Server *server, std::string target)
{
	int	i = 0;
	int	order = 0;
	std::string	check_double = "";
	Server::vecClient	clients_to_add;

	// check prefixes
	while (target[i] != '#' && target[i] != '&' && is_target_channel(target[i], "@%+")) {
		
		// check for double characters
		if (check_double.find_first_of(target[i]) != std::string::npos)
			return clients_to_add;
		else
			check_double.push_back(target[i]);
		
		// find operator order
		if (target[i] == '@' && order == 0)
			order = 1;
		else if (target[i] == '%' && order < 2)
			order = 2;
		else if (target[i] == '+' && order < 3)
			order = 3;
		i++;
	}
	
	// find name of the channel
	Channel	*channel = server->getChannel(target.substr(i, target.length() - i));
	if (!channel)
		return clients_to_add;
	
	// no external message mode
	if (channel->getModeStatus('n') && !channel->isClientInChannel(client->getNickName()))
		return clients_to_add;

	// moderated channel mode
	if (channel->getModeStatus('m')
		&& channel->getOperGrade(client->getNickName()) == 0)
		return clients_to_add;
	
	// find clients to add
	Channel::mapClients	channel_clients = channel->getClients();
	for (Channel::itMapClients it = channel_clients.begin(); it != channel_clients.end(); it++) {
		if (it->second.client == client)
			continue;
		else if (it->second.prefix.empty() && order < 1)
			clients_to_add.push_back(it->second.client);
		else if (it->second.prefix == "+" && order < 2)
			clients_to_add.push_back(it->second.client);
		else if (it->second.prefix == "%" && order < 3)
			clients_to_add.push_back(it->second.client);
		else if (it->second.prefix == "@" && order <= 3)
			clients_to_add.push_back(it->second.client);
	}
	
	return clients_to_add;
}

static Client	*client_to_add(Client *client, Server *server, std::string target)
{
	// if target has user@host format
	if (target.find_first_of("@") != std::string::npos) {

		if (!client->getModeStatus('o'))
			return NULL;
		
		size_t	pos = target.find_last_of(".");
		if (pos == std::string::npos
			|| (target[pos + 1] && (target[pos + 1] == '*'
			|| target[pos + 1] == '?')))
			return NULL;

		Server::vecClient	target_hosts = server->getClientsHost(target);
		
		if (target_hosts.empty())
			return NULL;
		else if (target_hosts.size() > 1)
			return NULL;
		else
			return target_hosts[0];
	}
	
	// if target is a nickname
	Client	*client_to_add = server->getClient(target);
	
	if (!client_to_add)
			return NULL;
	
	return client_to_add;
}

void notice(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "NOTICE command called" << RESET << std::endl;

	// if no target
	if (message.getParameters().empty())
		return ;
	
	// if no text to send
	if (message.getParameters().size() == 1)
		return ;
	
	// get all targets from recipient in one vector
	Message::vecString	targets = split(message.getParameters()[0], ",");
	
	// send message to all recipients
	for (Message::itVecString it = targets.begin(); it != targets.end(); it++) {

		// to check if <target> is the name of a client or the name of a channel
		if (is_target_channel(message.getParameters()[0][0], "#&@%+")) {
			
			// list of users to send the text
			Server::vecClient	list_clients = clients_from_channel(client, server, *it);
			
			//send message to all targets
			for (Server::itVecClient it_client = list_clients.begin(); it_client != list_clients.end(); it_client++) {
				server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
					client->getInet(), std::string("NOTICE"), *it + " :" + message.getParameters()[1]), 
					(*it_client)->getClientSocket());
			}
		}
		else {
			Client	*to_add = client_to_add(client, server, *it);
			if (to_add)
				server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
						client->getInet(), std::string("NOTICE"),
						to_add->getNickName() + " :" + message.getParameters()[1]), 
						to_add->getClientSocket());
		}
	}
}

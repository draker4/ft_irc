/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/05/04 12:08:36 by bperriol         ###   ########lyon.fr   */
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


static std::string	is_target_host(Server *server, std::string str)
{
	size_t	pos = str.find_first_of('#');
	size_t	pos_point = str.find_last_of('.');
	if (pos == std::string::npos || pos_point == std::string::npos
		|| pos_point < pos)
		return "";
	
	std::string	host = str.substr(pos + 1, host.length() - pos - 1);
	Server::mapClient	clients = server->getClients();
	for (Server::itMapClient it = clients.begin(); it != clients.end(); it++) {
		if (it->second->getInet() == host)
			return host;
	}
	return "";
}

static bool	is_target_channel(char c, std::string str)
{
	size_t	pos = str.find_first_of(c);
	if (pos != std::string::npos)
		return true;
	return false;
}

static void	clients_from_host(Client *client, Server *server, std::string target, std::string host, std::string message)
{
	Server::vecClient	to_add;

	// need to be operator to use the host mask
	if (!client->getModeStatus('o'))
		return ;
	
	// if nick#host format
	if (target[0] != '#') {
		std::string	nickname = target.substr(0, target.find_first_of('#'));
		
		Client	*to_send = server->getClient(nickname);
		if (!to_send)
			return ;
		
		if (to_send->getInet() != host)
			return ;
		
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
		client->getInet(), std::string("NOTICE"), to_send->getNickName() + " :" + message), 
			to_send->getClientSocket());
		return ;
	}

	// if #host fromat
	Server::mapClient	clients = server->getClients();

	for (Server::itMapClient it = clients.begin(); it != clients.end(); it++) {
		if (it->second->getInet() == host) {
			server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
			client->getInet(), std::string("NOTICE"),
			it->second->getNickName() + " :" + message), 
			it->second->getClientSocket());
		}
	}
}

static void	clients_from_channel(Client *client, Server *server, std::string target, std::string message)
{
	int	i = 0;
	int	order = 0;
	std::string	check_double = "";
	Server::vecClient	to_send;

	// check prefixes
	while (target[i] != '#' && target[i] != '&' && is_target_channel(target[i], "@%+")) {
		
		// check for double characters
		if (check_double.find_first_of(target[i]) != std::string::npos)
			return ;
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
		return ;
	
	// no external message mode
	if (channel->getModeStatus('n') && !channel->isClientInChannel(client->getNickName()))
		return ;

	// moderated channel mode
	if (channel->getModeStatus('m')
		&& channel->getOperGrade(client->getNickName()) == 0)
		return ;

	// if user is banned, the command is silently failed
	if (channel->isBanned(client))
		return ;
	
	// if channel is moderated and user is not voiced or operator
	if (channel->getModeStatus('m') && channel->getOperGrade(client->getNickName()) < 1)
		return ;
	
	// find clients to add
	Channel::mapClients	channel_clients = channel->getClients();
	for (Channel::itMapClients it = channel_clients.begin(); it != channel_clients.end(); it++) {
		if (it->second.client == client)
			continue;
		else if (it->second.prefix.empty() && order < 1)
			to_send.push_back(it->second.client);
		else if (it->second.prefix == "+" && order < 2)
			to_send.push_back(it->second.client);
		else if (it->second.prefix == "%" && order < 3)
			to_send.push_back(it->second.client);
		else if (it->second.prefix == "@" && order <= 3)
			to_send.push_back(it->second.client);
	}
	
	for (Server::itVecClient it_client = to_send.begin(); it_client != to_send.end(); it_client++) {
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
			client->getInet(), std::string("NOTICE"),
			channel->getName() + " :" + message), 
			(*it_client)->getClientSocket());
	}
}

static void	client_from_nick(Client *client, Server *server, std::string target, std::string message)
{
	// if target has nickname!*@* format
	if (target.find_first_of("!") != std::string::npos) {
		size_t	first_pos = target.find_first_of("!");
		size_t	second_pos = target.find_first_of("@");
		
		if (first_pos == std::string::npos || second_pos == std::string::npos)
			return ;

		Client	*destination = server->getClient(target.substr(0, first_pos));
		
		if (!destination)
			return ;
		
		std::string	username = target.substr(first_pos + 1, second_pos - first_pos - 1);
		
		if (destination->getUserName() != username)
			return ;
				
		std::string	host = target.substr(second_pos + 1, target.length() - second_pos);
		
		if (destination->getInet() != host)
			return ;
		
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
				client->getInet(), std::string("NOTICE"),
				destination->getNickName() + " :" + message), 
				destination->getClientSocket());
		return ;
	}
	
	// if target has user@host format
	if (target.find_first_of("@") != std::string::npos) {
		
		size_t	pos = target.find_last_of(".");
		if (pos == std::string::npos
			|| (target[pos + 1] && (target[pos + 1] == '*' || target[pos + 1] == '?')))
			return ;

		Server::vecClient	clients = server->getClientsHost(target);
		
		if (clients.empty())
			return ;
		
		if (clients.size() > 1)
			return ;
		
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
				client->getInet(), std::string("NOTICE"),
				clients[0]->getNickName() + " :" + message), 
				clients[0]->getClientSocket());
		return ;
	}
	
	// if target is a nickname
	Client	*to_send = server->getClient(target);
	
	if (!to_send)
		return ;
	
	server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
				client->getInet(), std::string("NOTICE"),
				to_send->getNickName() + " :" + message), 
				to_send->getClientSocket());
}

void notice(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "NOTICE command called" << RESET << std::endl;

	// if no target
	if (message.getParameters().empty())
		return ;

	// if target is the help bot
	Client	*bot = server->getHelpBot();
	if (bot && toUpper(message.getParameters()[0]) == toUpper(bot->getNickName())) {
		bot->sendHelp(client, server, message);
		return ;
	}
	
	// if no text to send
	if (message.getParameters().size() == 1)
		return ;
	
	// get all targets from recipient in one vector
	Message::vecString	targets = split(message.getParameters()[0], ",");
	
	// send message to all recipients
	for (Message::itVecString it = targets.begin(); it != targets.end(); it++) {

		std::string	host = is_target_host(server, (*it));

		if (!host.empty()) {
			if (DEBUG_COMMAND)
				std::cout << BLUE << "NOTICE sent to host" << RESET << std::endl;
			clients_from_host(client, server, *it, host, message.getParameters()[1]);
		}
		else if (is_target_channel((*it)[0], "#&@%+")) {
			if (DEBUG_COMMAND)
				std::cout << BLUE << "NOTICE sent to channel" << RESET << std::endl;
			clients_from_channel(client, server, *it, message.getParameters()[1]);
		}
		else {
			if (DEBUG_COMMAND)
				std::cout << BLUE << "NOTICE sent to nickname" << RESET << std::endl;
			client_from_nick(client, server, *it, message.getParameters()[1]);
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/28 19:11:57 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief PRIVMSG is used to send private messages between users, as well as to
 * send messages to channels.  <msgtarget> is usually the nickname of
 * the recipient of the message, or a channel name.
 * 
 * Parameters: <msgtarget> <text to be sent>
 * 
 * The <msgtarget> parameter may also be a host mask (#<mask>) or server
 * mask ($<mask>). In both cases the server will only send the PRIVMSG
 * to those who have a server or host matching the mask.  The mask MUST
 * have at least 1 (one) "." in it and no wildcards following the last
 * ".".  This requirement exists to prevent people sending messages to
 * "#*" or "$*", which would broadcast to all users.  Wildcards are the
 * '*' and '?'  characters.  This extension to the PRIVMSG command is
 * only available to operators.
 * Numeric Replies:
 *  	RPL_AWAY (301)
 * 		ERR_NOSUCHNICK (401)
 * 		ERR_CANNOTSENDTOCHAN (404)
 * 		ERR_TOOMANYTARGETS (407) 
 * 		ERR_NORECIPIENT (411)
 * 		ERR_NOTEXTTOSEND (412)
 * 		ERR_NOTOPLEVEL (413)
 * 		ERR_WILDTOPLEVEL (414)
 * @param server
 * @param client_fd User sending a msg
 * @param cmd_infos Structure w/ prefix, command name and message
 * 
 * EXAMPLES :
 * 		Examples:
 * 			:Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
 * 			; Message from Angel to Wiz.
 * 			PRIVMSG Angel :yes I'm receiving it !
 * 			; Command to send a message to Angel.
 * 			PRIVMSG jto@tolsun.oulu.fi :Hello !
 * 			; Command to send a message to a user on server tolsun.oulu.fi with
 * 			username of "jto".
 * 			PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
 * 			; Message to a user on server irc.stealth.net with username of "kalt", 
 * 			and connected from the host millennium.stealth.net.
 * 			PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
 * 			; Message to a user on the local server with username of "kalt", and
 * 			connected from the host millennium.stealth.net.
 * 			PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
 * 			; Message to the user with nickname Wiz who is connected from the host
 * 			tolsun.oulu.fi and has the username "jto".
 * 			PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
 * 			; Message to everyone on a server which has a name matching *.fi.
 * 			PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
 * 			; Message to all users who come from a host which has a name matching *.edu.
 * 
 * useful link :
 * 		- https://irssi.org/documentation/help/msg/
 * 		- https://modern.ircdocs.horse/#errnosuchnick-401
 * 		- http://abcdrfc.free.fr/rfc-vf/rfc1459.html (errors)
 * 		- https://askubuntu.com/questions/855881/irssi-where-do-private-messages-go (how to use IRSSI)
 * 		- https://datatracker.ietf.org/doc/html/rfc2812#section-3.3 RFC DE REFERENCE
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
		if (toUpper(it->second->getInet()) == toUpper(host))
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
	if (!client->getModeStatus('o')) {
		server->sendClient(ERR_NOPRIVILEGES(client->getNickName()), client->getClientSocket());
		return ;
	}
	
	// if nick#host format
	if (target[0] != '#') {
		std::string	nickname = target.substr(0, target.find_first_of('#'));
		
		Client	*to_send = server->getClient(nickname);
		if (!to_send) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target),
			client->getClientSocket());
			return ;
		}
		
		if (toUpper(to_send->getInet()) != toUpper(host)) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target),
			client->getClientSocket());
			return ;
		}
		
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
		client->getInet(), std::string("PRIVMSG"), to_send->getNickName() + " :" + message), 
			to_send->getClientSocket());
		return ;
	}

	// if #host format
	Server::mapClient	clients = server->getClients();

	for (Server::itMapClient it = clients.begin(); it != clients.end(); it++) {
		if (toUpper(it->second->getInet()) == toUpper(host)) {
			server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
			client->getInet(), std::string("PRIVMSG"),
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
		if (check_double.find_first_of(target[i]) != std::string::npos) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target.substr(i, target.length() - i)), 
			client->getClientSocket());
			return ;
		}
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
	if (!channel) {
		server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target.substr(i, target.length() - i)), 
		client->getClientSocket());
		return ;
	}
	
	// no external message mode
	if (channel->getModeStatus('n') && !channel->isClientInChannel(client->getNickName())) {
		server->sendClient(ERR_CANNOTSENDTOCHAN(client->getNickName(), channel->getName()), 
			client->getClientSocket());
		return ;
	}

	// moderated channel mode
	if (channel->getModeStatus('m')
		&& channel->getOperGrade(client->getNickName()) == 0) {
		server->sendClient(ERR_CANNOTSENDTOCHAN(client->getNickName(), channel->getName()), 
			client->getClientSocket());
		return ;
	}

	// if user is banned, the command is silently failed
	if (channel->isBanned(client))
		return ;
	
	// if channel is moderated and user is not voiced or operator
	if (channel->getModeStatus('m')
		&& channel->getOperGrade(client->getNickName()) < 1) {
		server->sendClient(ERR_CANNOTSENDTOCHAN(client->getNickName(), channel->getName()), 
			client->getClientSocket());
		return ;
	}
	
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
			client->getInet(), std::string("PRIVMSG"),
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
		
		if (first_pos == std::string::npos || second_pos == std::string::npos) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
				return ;
		}

		Client	*destination = server->getClient(target.substr(0, first_pos));
		
		if (!destination) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
				return ;
		}
		
		std::string	username = target.substr(first_pos + 1, second_pos - first_pos - 1);
		
		if (toUpper(destination->getUserName()) != toUpper(username)) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
				return ;
		}
				
		std::string	host = target.substr(second_pos + 1, target.length() - second_pos);
		
		if (toUpper(destination->getInet()) != toUpper(host)) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
				return ;
		}
		
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
				client->getInet(), std::string("PRIVMSG"),
				destination->getNickName() + " :" + message), 
				destination->getClientSocket());
		return ;
	}
	
	// if target has user@host format
	if (target.find_first_of("@") != std::string::npos) {
		
		size_t	pos = target.find_last_of(".");
		if (pos == std::string::npos
			|| (target[pos + 1] && (target[pos + 1] == '*' || target[pos + 1] == '?'))) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
			return ;
		}

		Server::vecClient	clients = server->getClientsHost(target);
		
		if (clients.empty()) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
			return ;
		}
		if (clients.size() > 1) {
			server->sendClient(ERR_TOOMANYTARGETS (target), client->getClientSocket());
			return ;
		}
		
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
				client->getInet(), std::string("PRIVMSG"),
				clients[0]->getNickName() + " :" + message), 
				clients[0]->getClientSocket());
		return ;
	}
	
	// if target is a nickname
	Client	*to_send = server->getClient(target);
	
	if (!to_send) {
		server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
			return ;
	}
	
	server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
				client->getInet(), std::string("PRIVMSG"),
				to_send->getNickName() + " :" + message), 
				to_send->getClientSocket());
}

void privmsg(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "PRIVMSG command called" << RESET << std::endl;

	// if no target
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NORECIPIENT(client->getNickName(), std::string("PRIVMSG")), 
			client->getClientSocket());
		return ;
	}

	// if target is the help bot
	Client	*bot = server->getHelpBot();
	if (bot && toUpper(message.getParameters()[0]) == toUpper(bot->getNickName())) {
		bot->sendHelp(client, server, message);
		return ;
	}
	
	// if no text to send
	if (message.getParameters().size() == 1) {
		server->sendClient(ERR_NOTEXTTOSEND(client->getNickName()), 
			client->getClientSocket());
		return ;
	}
	
	// get all targets from recipient in one vector
	Message::vecString	targets = split(message.getParameters()[0], ",");
	
	// send message to all recipients
	for (Message::itVecString it = targets.begin(); it != targets.end(); it++) {

		std::string	host = is_target_host(server, (*it));

		if (!host.empty()) {
			if (DEBUG_COMMAND)
				std::cout << BLUE << "PRIVMSG sent to host" << RESET << std::endl;
			clients_from_host(client, server, *it, host, message.getParameters()[1]);
		}
		else if (is_target_channel((*it)[0], "#&@%+")) {
			if (DEBUG_COMMAND)
				std::cout << BLUE << "PRIVMSG sent to channel" << RESET << std::endl;
			clients_from_channel(client, server, *it, message.getParameters()[1]);
		}
		else {
			if (DEBUG_COMMAND)
				std::cout << BLUE << "PRIVMSG sent to nickname" << RESET << std::endl;
			client_from_nick(client, server, *it, message.getParameters()[1]);
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/25 11:22:09 by bperriol         ###   ########lyon.fr   */
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
		if (check_double.find_first_of(target[i]) != std::string::npos) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target.substr(i, target.length() - i)), 
			client->getClientSocket());
			return clients_to_add;
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
		return clients_to_add;
	}
	
	// no external message mode
	if (channel->getModeStatus('n') && !channel->isClientInChannel(client->getNickName())) {
		server->sendClient(ERR_CANNOTSENDTOCHAN(client->getNickName(), channel->getName()), 
			client->getClientSocket());
		return clients_to_add;
	}

	// moderated channel mode
	if (channel->getModeStatus('m')
		&& channel->getOperGrade(client->getNickName()) == 0) {
		server->sendClient(ERR_CANNOTSENDTOCHAN(client->getNickName(), channel->getName()), 
			client->getClientSocket());
		return clients_to_add;
	}
	
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

		if (!client->getModeStatus('o')) {
			server->sendClient(ERR_NOPRIVILEGES(client->getNickName()), client->getClientSocket());
			return NULL;
		}
		
		size_t	pos = target.find_last_of(".");
		if (pos == std::string::npos
			|| (target[pos + 1] && (target[pos + 1] == '*' || target[pos + 1] == '?'))) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
			return NULL;
		}

		Server::vecClient	target_hosts = server->getClientsHost(target);
		
		if (target_hosts.empty()) {
			server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
			return NULL;
		}
		else if (target_hosts.size() > 1) {
			server->sendClient(ERR_TOOMANYTARGETS (target), client->getClientSocket());
			return NULL;
		}
		else
			return target_hosts[0];
	}
	
	// if target is a nickname
	Client	*client_to_add = server->getClient(target);
	
	if (!client_to_add) {
		server->sendClient(ERR_NOSUCHNICK(client->getNickName(), target), client->getClientSocket());
			return NULL;
	}
	
	return client_to_add;
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

		// to check if <target> is the name of a client or the name of a channel
		if (is_target_channel(message.getParameters()[0][0], "#&@%+")) {
			
			// list of users to send the text
			Server::vecClient	list_clients = clients_from_channel(client, server, *it);
			
			//send message to all targets
			for (Server::itVecClient it_client = list_clients.begin(); it_client != list_clients.end(); it_client++) {
				server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
					client->getInet(), std::string("PRIVMSG"), *it + " :" + message.getParameters()[1]), 
					(*it_client)->getClientSocket());
			}
		}
		else {
			Client	*to_add = client_to_add(client, server, *it);
			if (to_add)
				server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(),
						client->getInet(), std::string("PRIVMSG"),
						to_add->getNickName() + " :" + message.getParameters()[1]), 
						to_add->getClientSocket());
		}
	}
}

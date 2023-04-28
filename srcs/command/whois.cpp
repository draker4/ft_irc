/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 18:34:38 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 20:05:08 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The WHOIS command is used to query information about particular users. The server will
 * answer this command with several numeric messages with information about the nicks,
 * ending with RPL_ENDOFWHOIS.
 * Servers MUST end their reply to WHOIS messages with one of these numerics: 
 * 	ERR_NOSUCHNICK (401)
 * 	ERR_NONICKNAMEGIVEN (431)
 * 	RPL_ENDOFWHOIS (318)
 * 
 * The following numerics MAY be returned as part of the whois reply:
 * 	RPL_WHOISREGNICK (307)
 * 	RPL_WHOISUSER (311)
 * 	RPL_WHOISSERVER (312)
 * 	RPL_WHOISOPERATOR (313)
 * 	RPL_WHOISCHANNELS (319)
 * 	RPL_WHOISHOST (378)
 * 	RPL_WHOISMODES (379)
 * 
 * Servers typically send some of these numerics only to the client itself and to servers operators,
 * as they contain privacy-sensitive information that should not be revealed to other users.
 * 
 * Syntax : WHOIS <nickname>
 * 
 */
void whois(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "INVITE command called" << RESET << std::endl; // if no arguments are given
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NONICKNAMEGIVEN(client->getNickName()),
			client->getClientSocket());
	} else if (!server->isClientInServer(message.getParameters()[0])) // if the nickname given doesn't exist
		server->sendClient(ERR_NOSUCHNICK(client->getNickName(), message.getParameters()[0]),
			client->getClientSocket());
	else {
		Client *target = server->getClient(message.getParameters()[0]);
		// is the user is registered
		if (target->getModeStatus('r'))
			server->sendClient(RPL_WHOISREGNICK(client->getNickName(), target->getNickName()),
				client->getClientSocket());
		// User info
		server->sendClient(RPL_WHOISUSER(client->getNickName(), target->getNickName(),
			target->getUserName(), target->getInet(), target->getRealName()),
			client->getClientSocket());
		// Server info
		server->sendClient(RPL_WHOISSERVER(client->getNickName(), target->getNickName(),
			SERVERNAME, SERVERINFO), client->getClientSocket());
		// if the user is an operator
		if (target->getModeStatus('o'))
			server->sendClient(RPL_WHOISOPERATOR(client->getNickName(), target->getNickName()),
				client->getClientSocket());
		// User channels list
			//funtiond to build the list and pass it to the message
		// if (!target->getChannels().empty()) {
		// 	std::string channels;
		// 	for (Client::itVecChannel it = target->getChannels().begin();
		// 		it != target->getChannels().end(); it++) {
				
		// 		channels += (*it)->getName();
		// 		if (it + 1 != target->getChannels().end())
		// 			channels += " ";
		// 	}
		// 	server->sendClient(RPL_WHOISCHANNELS(client->getNickName(), target->getNickName(),
		// 		channels), client->getClientSocket());
		// }
		// 	server->sendClient(RPL_WHOISCHANNELS(client->getNickName(), target->getNickName(),
		// 		target->getChannels()), client->getClientSocket());
		if (client->getModeStatus('o')) {
			// server->sendClient(RPL_WHOISHOST(client->getNickName(), target->getNickName(),
			// 	target->getInet()),
			// 	client->getClientSocket());
			// server->sendClient(RPL_WHOISMODES(client->getNickName(), target->getNickName(),
			// 	target->getMode()),
			// 	client->getClientSocket());
		}
		server->sendClient(RPL_ENDOFWHOIS(client->getNickName(), target->getNickName()),
			client->getClientSocket());
	}
}
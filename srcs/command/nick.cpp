/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/13 18:21:02 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The NICK command is used to give the client a nickname or 
 * 		change the previous one.
 * 
 * 	Syntax: NICK <nickname>
 * 
 * 	Nicknames are non-empty strings with the following restrictions:
 * 	
 * 	They MUST NOT contain any of the following characters: 
 * 	- space (' '), 
 * 	- comma (','), 
 * 	- asterisk ('*'), 
 * 	- question mark ('?'), 
 * 	- exclamation mark ('!'), 
 * 	- at sign ('@'),
 * 	- dot ('.').
 * 	
 * 	They MUST NOT start with any of the following characters: 
 * 	dollar ('$'), colon (':'), diese (#), ampersand(&).
 * 	
 * 	Numeric Replies:
 * 
 * 	ERR_NONICKNAMEGIVEN (431)
 * 	ERR_ERRONEUSNICKNAME (432)
 * 	ERR_NICKNAMEINUSE (433)
 * 	ERR_NICKCOLLISION (436)
 * 
 * 	Example:
 * 	[CLIENT] /Nick mike
 * 
 */

bool invalidChar(std::string nickname)
{
	if (nickname.find_first_of(" ,*?!@.") != std::string::npos
		|| nickname[0] == '$' || nickname[0] == ':'
		|| nickname[0] == '#' || nickname[0] == '&')
		return (true);
	return (false);
}

bool	isAlreadyUsed(Server *server, Client *client, std::string nickname)
{
	Server::mapClient	clientList = server->getClients();
	for (Server::itMapClient itClient = clientList.begin();
		itClient != clientList.end(); itClient++) {
		if (itClient->second->getClientSocket() != client->getClientSocket()
			&& itClient->second->getNickName() == nickname)
			return (true);
	}
	return (false);
}

void nick(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "NICK command called" << RESET << std::endl;
	
	if (!client->getPasswordStatus()) {
		server->sendClient(ERROR_MESSAGE(std::string("Usage: PASS, NICK, USER.")),
		client->getClientSocket());
	}
	else {
		if (message.getParameters().empty()) {
			server->sendClient(ERR_NONICKNAMEGIVEN, client->getClientSocket());
			return;
		} else if (invalidChar(message.getParameters()[0])) {
			server->sendClient(ERR_ERRONEUSNICKNAME(message.getParameters()[0]), 
				client->getClientSocket());
		} else if (isAlreadyUsed(server, client, message.getParameters()[0])) {
			server->sendClient(ERR_NICKNAMEINUSE(message.getParameters()[0]), 
				client->getClientSocket());
		} else {
			client->setOldNickName(client->getNickName());
			client->setNickName(message.getParameters()[0]);
			if (!client->getRegistered() && client->getUserName().size()) {
				client->setRegistered(true);
				server->sendWelcome(client);
			} else {
				server->sendClient(RPL_NICK(client->getOldNickName(), client->getNickName(),
					client->getUserName(), client->getInet()), client->getClientSocket());
			}
		}
	}
}

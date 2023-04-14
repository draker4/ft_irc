/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:26 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The MODE command is used to set or remove options (or modes) from a given target.
 * 
 * Parameters: <target> [<modestring> [<mode arguments>...]]
 * 
 * <modestring> starts with a plus ('+', 0x2B) or minus ('-', 0x2D) character, and is made up of the following characters:
 * '+': Adds the following mode(s).
 * '-': Removes the following mode(s).
 * 'a-zA-Z': Mode letters, indicating which modes are to be added/removed.
 * 
 * User case :
 * If <modestring> is given, the supplied modes will be applied : a MODE message will be sent to the user containing the changed modes. 
 * If one or more modes sent are not implemented on the server, 
 * the server MUST apply the modes that are implemented, and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.
 * 
 * Channel case :
 * syntax command : /mode <channel> <+ ou -> <mode> [parametres]
 * If the user has permission to change modes on the target, the supplied modes will be applied based on the type of the mode (see below). 
 * For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>. 
 * - If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode. 
 * - If a type A mode has been sent without an argument : 
 * the contents of the list MUST be sent to the user, 
 * unless it contains sensitive information the user is not allowed to access. 
 * When the server is done processing the modes, a MODE command is sent to all members of the channel containing the mode changes. 
 * Servers MAY choose to hide sensitive information when sending the mode changes.
 * 
 * Exemples :
 * - +o : /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
 * Message à send aux users du channel : ':irc.example.com MODE #cool +o MEAT'; 
 * The irc.example.com server gave channel operator privileges to MEAT on #cool.
 * - +k : /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
 * - +s : /mode #cool +s (le channel devient secret)
 * - plusieur modes : /mode #cool +ts (le channel est en mode +t +s)
 * 
 * @param server
 * @param client_fd User sending a msg
 * @param cmd_infos Structure w/ prefix, command name and message
 * 
 * Useful link : https://modern.ircdocs.horse/#mode-message
 * https://www.techbull.com/techbull/guide/internet/irccommande.html
 * 
 */


void removeModeClient(Client *client, Server *server, char mode)
{
	if (client->getModeStatus(mode)) {				
		client->removeMode(mode);
		server->sendClient(RPL_MODE(client->getNickName(), client->getUserName(),
			client->getInet(), "-", mode), client->getClientSocket());
	}
}

void addModeClient(Client *client, Server *server, char mode)
{
	if (!client->getModeStatus(mode)) {
		client->addMode(mode);
		server->sendClient(RPL_MODE(client->getNickName(), client->getUserName(),
			client->getInet(), "+", mode), client->getClientSocket());
	}
}

void userAddMode(Client *client, const Message &message, Server *server, size_t *i)
{
	size_t j = *i + 1;
	while(j < message.getParameters()[1].size() && message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		if (message.getParameters()[1][j] == 'r') {
			addModeClient(client, server, 'r');
		} else if (message.getParameters()[1][j] == 'w') {
			addModeClient(client, server, 'w');
		} else if (message.getParameters()[1][j] == 'i') {
			addModeClient(client, server, 'i');
		} else {
			server->sendClient(ERR_UMODEUNKNOWNFLAG(client->getNickName()),
				client->getClientSocket());
		}
		j++;
	}
	*i = j;
}

void userRemoveMode(Client *client, const Message &message, Server *server, size_t *i)
{
	size_t j = *i + 1;
	while(j < message.getParameters()[1].size()
		&& message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		if (message.getParameters()[1][j] == 'r') {
			removeModeClient(client, server, 'r');
		} else if (message.getParameters()[1][j] == 'w') {
			removeModeClient(client, server, 'w');
		} else if (message.getParameters()[1][j] == 'i') {
			removeModeClient(client, server, 'i');
		} else {
			server->sendClient(ERR_UMODEUNKNOWNFLAG(client->getNickName()),
				client->getClientSocket());
		}
		j++;
	}
	*i = j;
}

void userMode(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "MODE for user" << RESET << std::endl;
	Client *clientModed = server->getClient(message.getParameters()[0]);
	if (!clientModed) { // if client doesn't exist : ERR_NOSUCHNICK
		server->sendClient(ERR_NOSUCHNICK(client->getNickName(),
			message.getParameters()[0]), client->getClientSocket());
	} else if (client->getNickName() != message.getParameters()[0]) { // if client is not the sender : ERR_USERSDONTMATCH
		server->sendClient(ERR_USERSDONTMATCH(client->getNickName()),
			client->getClientSocket());
	} else if (message.getParameters().size() == 1) { // if no mode is given : send current mode
		server->sendClient(RPL_UMODEIS(client->getNickName(), client->getMode()),
			client->getClientSocket());
	} else { // handle mode changment
		size_t i = 0;
		while(i < message.getParameters()[1].size()) {
			if (message.getParameters()[1][i] == '+') { // add mode
				userAddMode(client, message, server, &i);
			} else if (message.getParameters()[1][i] == '-') { // remove mode
				userRemoveMode(client, message, server, &i);
			} else {
				server->sendClient(ERR_UMODEUNKNOWNFLAG(client->getNickName()),
					client->getClientSocket());
				return;
				i++;
			}
		}
	}
}

void channelMode (Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "MODE for channel" << RESET << std::endl;
	Channel *channelModed = server->getChannel(message.getParameters()[0]);
	if (!channelModed) { // if channel doesn't exist : ERR_NOSUCHNICK
		server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(),
			message.getParameters()[0]), client->getClientSocket());
	} else if (message.getParameters().size() == 1) { 
		server->sendClient(RPL_CHANNELMODEIS(client->getNickName(),
			channelModed->getName(), channelModed->getMode()),
			client->getClientSocket());
	}
}

void mode(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "MODE command called" << RESET << std::endl;
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), "MODE"),
		client->getClientSocket());
	} else if (message.getParameters()[0][0] == '#'
		|| message.getParameters()[0][0] == '&') {
		channelMode (client, message, server);
	} else {
		userMode (client, message, server);
	}
}

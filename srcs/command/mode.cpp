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

/* -----------------------------  Mode on User  ----------------------------- */

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
	size_t j = *i;
	while(j < message.getParameters()[1].size() && message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		case 'r': // r : user is a registered user
			addModeClient(client, server, 'r');
			break;
		case 'w': // w : user receives wallops
			addModeClient(client, server, 'w');
			break;
		case 'i': // i : marks a users as invisible
			addModeClient(client, server, 'i');
			break;
		default:
			server->sendClient(ERR_UMODEUNKNOWNFLAG(client->getNickName()),
				client->getClientSocket());
			break;
		}
		j++;
	}
	*i = j;
}

void userRemoveMode(Client *client, const Message &message, Server *server, size_t *i)
{
	size_t j = *i + 1;
	while(j < message.getParameters()[1].size() && message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		case 'r': // r : user is a registered user
			removeModeClient(client, server, 'r');
			break;
		case 'w': // w : user receives wallops
			removeModeClient(client, server, 'w');
			break;
		case 'i': // i : marks a users as invisible
			removeModeClient(client, server, 'i');
			break;
		default:
			server->sendClient(ERR_UMODEUNKNOWNFLAG(client->getNickName()),
				client->getClientSocket());
			break;
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
	// if client doesn't exist : ERR_NOSUCHNICK
	if (!clientModed) {
		server->sendClient(ERR_NOSUCHNICK(client->getNickName(),
			message.getParameters()[0]), client->getClientSocket());
	}
	// if client is not the sender : ERR_USERSDONTMATCH
	else if (client->getNickName() != message.getParameters()[0]) {
		server->sendClient(ERR_USERSDONTMATCH(client->getNickName()),
			client->getClientSocket());
	}
	// if no mode is given : send current mode for the user
	else if (message.getParameters().size() == 1) {
		server->sendClient(RPL_UMODEIS(client->getNickName(), client->getMode()),
			client->getClientSocket());
	} 
	// handle mode changment
	else {
		size_t i = 0;
		while(i < message.getParameters()[1].size()) {
			if (message.getParameters()[1][i] == '+') { // add mode
				i++;
				userAddMode(client, message, server, &i);
			} else if (message.getParameters()[1][i] == '-') { // remove mode
				userRemoveMode(client, message, server, &i);
			} else {
				userAddMode(client, message, server, &i);
			}
		}
	}
}

/* ---------------------------  Mode on Channel  ---------------------------- */

// void removeUserModeChannel(Client *client, Server *server, Channel *channel, char mode, std::string paramModeName)
// {
// 	if (channel->getUserModeStatus(mode, paramModeName)) {				
// 		channel->removeUserMode(mode, paramModeName);
// 		//find message
// 			// server->sendClient(RPL_MODE(client->getNickName(), channel->getName(),
// 			// 	client->getInet(), "-", mode), client->getClientSocket());
// 		//should send to all users in the channel
// 	}
// }

// void addUserModeChannel(Client *client, Server *server, Channel *channel, char mode, std::string paramModeName)
// {
// 	if (!channel->getUserModeStatus(mode, paramModeName)) {
// 		channel->addUserMode(mode, paramModeName);
// 		//find message
// 			// server->sendClient(RPL_MODE(client->getNickName(), channel->getName(),
// 			// 	client->getInet(), "+", mode), client->getClientSocket());
// 		//should send to all users in the channel
// 	}
// }

void removeModeChannel(Client *client, Server *server, Channel *channel, char mode)
{
	if (channel->getModeStatus(mode)) {				
		channel->removeMode(mode);
		server->sendClient(RPL_MODE(client->getNickName(), channel->getName(),
			client->getInet(), "-", mode), client->getClientSocket());
		//should send to all users in the channel
	}
}

void addModeChannel(Client *client, Server *server, Channel *channel, char mode)
{
	if (!channel->getModeStatus(mode)) {
		channel->addMode(mode);
		server->sendClient(RPL_MODE(client->getNickName(), channel->getName(),
			client->getInet(), "+", mode), client->getClientSocket());
		//should send to all users in the channel
	}
}

void channelAddMode(Client *client, const Message &message, Server *server, Channel *channel, size_t *i, size_t *modeArg)
{
	size_t j = *i;
	(void)modeArg;
	while(j < message.getParameters()[1].size() && message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		
		case 'i': // i : set the channel to invite only
			addModeChannel(client, server, channel, 'i');
			break;
		case 'n': // n : set the channel to no external messages
			addModeChannel(client, server, channel, 'n');
			break;
		case 't': // t : only ops can change the topic
			addModeChannel(client, server, channel, 't');
			break;
		case 'm': // m : only ops can send messages to the channel
			addModeChannel(client, server, channel, 'm');
			break;
		case 's': // s : set the channel to secret
			addModeChannel(client, server, channel, 's');
			break;
		case 'p': // p : set the channel to private
			addModeChannel(client, server, channel, 'p');
			break;
		case 'k': // k : set the channel key (required the password in argument)
			addModeChannel(client, server, channel, 'k');
			// TODO : add the password in the channel and do nothing if it is not
			break;
		case 'l': // l : set the limit of users in the channel (required the limit in argument)
			addModeChannel(client, server, channel, 'l');
			// TODO : add the limit in the channel and do nothing if it is not
			break;
		// case 'b': // b : user is banned from the channel (required the mask/user in argument)
		// 	addUserModeChannel(client, server, channel, 'b');
		// 	// TODO : add the mask/user in the ban list if arguments with user connected to the channel
		// 	// TODO : send the ban list to the client if no arguments
		// 	// have to be on the user/channel mode
		// 	break;
		// case 'q': // q : give channel owner privileges to a user (required the user in argument)
		// 	addUserModeChannel(client, server, channel, 'q');
		// 	// have to be on the user/channel mode and must be grade 3
		// 	break;
		// case 'o': // o : give channel operator privileges to a user	(required the user in argument)
		// 	addUserModeChannel(client, server, channel, 'o');
		// 	// have to be on the user/channel mode
		// 	break;
		// case 'h': // h : give channel half-operator privileges to a user (required the user in argument)
		// 	addUserModeChannel(client, server, channel, 'h');
		// 	// have to be on the user/channel mode
		// 	break;
		// case 'v': // v : give channel voice to a user (required the user in argument)
		// 	addUserModeChannel(client, server, channel, 'v');
		// 	// have to be on the user/channel mode
		// 	break;
		default:
			server->sendClient(ERR_UMODEUNKNOWNFLAG(client->getNickName()),
				client->getClientSocket());
			break;
	}
		j++;
	}
	*i = j;
}

void channelRemoveMode(Client *client, const Message &message, Server *server, Channel *channel, size_t *i, size_t *modeArg)
{
	size_t j = *i + 1;
	(void)modeArg;
	while(j < message.getParameters()[1].size()
		&& message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		case 'i': // i : set the channel to invite only
			removeModeChannel(client, server, channel, 'i');
			break;
		case 'n': // n : set the channel to no external messages
			removeModeChannel(client, server, channel, 'n');
			break;
		case 't': // t : only ops can change the topic
			removeModeChannel(client, server, channel, 't');
			break;
		case 'm': // m : only ops can send messages to the channel
			removeModeChannel(client, server, channel, 'm');
			break;
		case 's': // s : set the channel to secret
			removeModeChannel(client, server, channel, 's');
			break;
		case 'p': // p : set the channel to private
			removeModeChannel(client, server, channel, 'p');
			break;
		case 'k': // k : set the channel key (required the password in argument)
			removeModeChannel(client, server, channel, 'k');
			break;
		case 'l': // l : set the limit of users in the channel (required the limit in argument)
			removeModeChannel(client, server, channel, 'l');
			break;
		// case 'b': // b : user is banned from the channel (required the mask/user in argument)
		// 	removeUserModeChannel(client, server, channel, 'b');
		// 	break;
		// case 'q': // q : give channel owner privileges to a user (required the user in argument)
		// 	removeUserModeChannel(client, server, channel, 'q');
		// 	break;
		// case 'o': // o : give channel operator privileges to a user	(required the user in argument)
		// 	removeUserModeChannel(client, server, channel, 'o');
		// 	break;
		// case 'h': // h : give channel half-operator privileges to a user (required the user in argument)
		// 	removeUserModeChannel(client, server, channel, 'h');
		// 	break;
		// case 'v': // v : give channel voice to a user (required the user in argument)
		// 	removeUserModeChannel(client, server, channel, 'v');
		// 	break;
		default:
			server->sendClient(ERR_UMODEUNKNOWNFLAG(client->getNickName()),
				client->getClientSocket());
			break;
	}
		j++;
	}
	*i = j;
}

void channelMode (Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "MODE for channel" << RESET << std::endl;
	Channel *channelModed = server->getChannel(message.getParameters()[0]);
	// if channel doesn't exist : ERR_NOSUCHNICK
	if (!channelModed) {
		server->sendClient(ERR_NOSUCHCHANNEL(client->getNickName(),
			message.getParameters()[0]), client->getClientSocket());
	}
	// if no mode is given : send current mode for the channel
	else if (message.getParameters().size() == 1) { //ADD the arguments to give in the message in RPL_CHANNELMODEIS
		server->sendClient(RPL_CHANNELMODEIS(client->getNickName(),
			channelModed->getName(), channelModed->getMode()), client->getClientSocket());
		server->sendClient(RPL_CREATIONTIME(client->getNickName(),
			channelModed->getName(), channelModed->getTimeCreated()), client->getClientSocket());
	}
	// handle mode changment
	else {
		size_t i = 0;
		// Must be an op to change the mode
		if (!channelModed->getOperGrade(client->getNickName())) {
			server->sendClient(ERR_CHANOPRIVSNEEDED(client->getNickName(),
				channelModed->getName()), client->getClientSocket());
			return;
		}
		size_t modeArg = 2;
		while(i < message.getParameters()[1].size()) {
			if (message.getParameters()[1][i] == '+') { // add mode
				i++;
				channelAddMode(client, message, server, channelModed, &i, &modeArg);
			} else if (message.getParameters()[1][i] == '-') { // remove mode
				channelRemoveMode(client, message, server, channelModed, &i, &modeArg);
			} else {
				channelAddMode(client, message, server, channelModed, &i, &modeArg);
			}
		}
	}
}

/* ---------------------------------  MODE  --------------------------------- */

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboisson <bboisson@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/26 14:26:40 by bboisson         ###   ########lyon.fr   */
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
 */

/* -----------------------------  Mode on User  ----------------------------- */

void removeModeClient(Client *client, Server *server, char mode)
{
	if (client->getModeStatus(mode)) {				
		client->removeMode(mode);
		server->sendClient(RPL_MODE_USER(client->getNickName(), client->getUserName(),
			client->getInet(), "-", mode), client->getClientSocket());
	}
}

void addModeClient(Client *client, Server *server, char mode)
{
	if (!client->getModeStatus(mode)) {
		client->addMode(mode);
		server->sendClient(RPL_MODE_USER(client->getNickName(), client->getUserName(),
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
		Channel::mapClients	clients = channel->getClients();
		for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
			server->sendClient(RPL_MODE_CHANNEL(client->getNickName(), client->getUserName(),
				client->getInet(), channel->getName(), "-", mode),
				it->second.client->getClientSocket());
		}
	}
}

void addModeChannel(Client *client, Server *server, Channel *channel, char mode)
{
	if (!channel->getModeStatus(mode)) {
		channel->addMode(mode);
		Channel::mapClients	clients = channel->getClients();
		if (mode == 'k') {
			for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
				server->sendClient(RPL_MODE_CHANNEL_KEY(client->getNickName(), client->getUserName(),
					client->getInet(), channel->getName(), "+", channel->getKey()),
					it->second.client->getClientSocket());
			}
		} else {
			for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
				server->sendClient(RPL_MODE_CHANNEL(client->getNickName(), client->getUserName(),
					client->getInet(), channel->getName(), "+", mode),
					it->second.client->getClientSocket());
			}
		}
	}
}

void channelAddMode(Client *client, const Message &message, Server *server, Channel *channel, size_t *i, size_t *modeArg)
{
	size_t j = *i;
	(void)modeArg;
	while(j < message.getParameters()[1].size() && message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		
		case 'i': // i : set the channel to invite only (operator only)
			if (channel->getOperGrade(client->getNickName()) == 3)
				addModeChannel(client, server, channel, 'i');
			break;
		case 'n': // n : set the channel to no external messages (half-operator and +)
			addModeChannel(client, server, channel, 'n');
			break;
		case 't': // t : only ops can change the topic (half-operator and +)
			addModeChannel(client, server, channel, 't');
			break;
		case 'm': // m : only ops can send messages to the channel (half-operator and +)
			addModeChannel(client, server, channel, 'm');
			break;
		case 's': // s : set the channel to secret (operator only)
			if (channel->getOperGrade(client->getNickName()) == 3)
				addModeChannel(client, server, channel, 's');
			break;
		case 'p': // p : set the channel to private (operator only)
			if (channel->getOperGrade(client->getNickName()) == 3)
				addModeChannel(client, server, channel, 'p');
			break;
		case 'k': // k : set the channel key (required the password in argument) (half-operator and +)
			if (message.getParameters().size() > *modeArg && channel->getKey().empty()) {
				if (!channel->getModeStatus('k')) {	
					std::cout << message.getParameters()[*modeArg] << std::endl;
					channel->setKey(message.getParameters()[*modeArg]);			
					addModeChannel(client, server, channel, 'k');
					(*modeArg)++;
				}
			}
			break;
		case 'l': // l : set the limit of users in the channel (required the limit in argument) (operator only)
			if (message.getParameters().size() > *modeArg) {
				addModeChannel(client, server, channel, 'l');
				channel->setClientLimit(message.getParameters()[*modeArg]);
				(*modeArg)++;
			}
			break;
		// case 'b': // b : user is banned from the channel (required the mask/user in argument)
		// 	addUserModeChannel(client, server, channel, 'b');
		// 	// TODO : add the mask/user in the ban list if arguments with user connected to the channel
		// 	// TODO : send the ban list to the client if no arguments
		// 	// have to be on the user/channel mode
		// 	break;
		// case 'o': // o : give channel operator privileges to a user	(required the user in argument) (operator only)
		// 	addUserModeChannel(client, server, channel, 'o');
		// 	// have to be on the user/channel mode
		// 	break;
		// case 'h': // h : give channel half-operator privileges to a user (required the user in argument) (half-operator and +)
		// 	addUserModeChannel(client, server, channel, 'h');
		// 	// have to be on the user/channel mode
		// 	break;
		// case 'v': // v : give channel voice to a user (required the user in argument) (half-operator and +)
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
		case 'i': // i : remove the channel mode invite only (operator only)
			removeModeChannel(client, server, channel, 'i');
			break;
		case 'n': // n : remove the channel mode no external messages (half-operator and +)
			removeModeChannel(client, server, channel, 'n');
			break;
		case 't': // t : remove the channel mode that only ops can change the topic (half-operator and +)
			removeModeChannel(client, server, channel, 't');
			break;
		case 'm': // m : remove the channel mode that only ops can send messages to the channel (half-operator and +)
			removeModeChannel(client, server, channel, 'm');
			break;
		case 's': // s : remove the channel mode secret (operator only)
			removeModeChannel(client, server, channel, 's');
			break;
		case 'p': // p : remove the channel mode private (operator only)
			removeModeChannel(client, server, channel, 'p');
			break;
		case 'k': // k : remove the channel key (required the password defined in argument) (half-operator and +)
			if (message.getParameters().size() > *modeArg) {
				if (channel->getKey() == message.getParameters()[*modeArg]) {
					channel->setKey("");
					removeModeChannel(client, server, channel, 'k');
				}
				(*modeArg)++;
			}
			break;
		case 'l': // l : remove the channel mode limit of users in the channel (required the limit in argument) (operator only)
			removeModeChannel(client, server, channel, 'l');
			break;
		// case 'b': // b : remove the user banned from the channel (required the mask/user in argument)
		// 	removeUserModeChannel(client, server, channel, 'b');
		// 	break;
		// case 'o': // o : remove the channel operator privileges to a user	(required the user in argument) (operator only)
		// 	removeUserModeChannel(client, server, channel, 'o');
		// 	break;
		// case 'h': // h : remove the channel half-operator privileges to a user (required the user in argument) (half-operator and +)
		// 	removeUserModeChannel(client, server, channel, 'h');
		// 	break;
		// case 'v': // v : remove the channel voice to a user (required the user in argument) (half-operator and +)
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

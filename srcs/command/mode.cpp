/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/28 13:50:21 by bperriol         ###   ########lyon.fr   */
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

void addModeClient(Client *client, Server *server, char mode)
{
	if (!client->getModeStatus(mode)) {
		client->addMode(mode);
		server->sendClient(RPL_MODE_USER(client->getNickName(), client->getUserName(),
			client->getInet(), "+", mode), client->getClientSocket());
	}
}

void removeModeClient(Client *client, Server *server, char mode)
{
	if (client->getModeStatus(mode)) {				
		client->removeMode(mode);
		server->sendClient(RPL_MODE_USER(client->getNickName(), client->getUserName(),
			client->getInet(), "-", mode), client->getClientSocket());
	}
}

void userAddMode(Client *client, const Message &message, Server *server, size_t *i)
{
	size_t j = *i;
	while(j < message.getParameters()[1].size() && message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		case 'w': // w : user receives wallops
		case 'i': // i : marks a users as invisible
		case 'r': // r : user is a registered user
			addModeClient(client, server, message.getParameters()[1][j]);
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
		case 'w': // w : user receives wallops
		case 'i': // i : remove users as invisible
			removeModeClient(client, server, message.getParameters()[1][j]);
			break;
		case 'r': // r : remove user as a registered user
		case 'o': // o : remove users as an IRC operator
			if (client->getModeStatus('o'))
				removeModeClient(client, server, message.getParameters()[1][j]);
			else
				server->sendClient(ERR_NOPRIVILEGES(client->getNickName()),
					client->getClientSocket());
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
	else if (toUpper(client->getNickName()) != toUpper(message.getParameters()[0])) {
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

void addUserModeChannel(Client *client, Server *server, Channel *channel, char mode, std::string nickName)
{
	if (channel->isClientInChannel(nickName) && !channel->getUserModeStatus(nickName, mode)) {
		channel->addUserMode(nickName, mode);
		Channel::mapClients	clients = channel->getClients();
		for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
			server->sendClient(RPL_MODE_CHANNEL_PARAM(client->getNickName(), client->getUserName(),
				client->getInet(), channel->getName(), "+", mode, nickName),
				it->second.client->getClientSocket());
		}
	}
}

void removeUserModeChannel(Client *client, Server *server, Channel *channel, char mode, std::string nickName)
{
	if (channel->isClientInChannel(nickName) && channel->getUserModeStatus(nickName, mode)) {				
		channel->removeUserMode(nickName, mode);
		Channel::mapClients	clients = channel->getClients();
		for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
			server->sendClient(RPL_MODE_CHANNEL_PARAM(client->getNickName(), client->getUserName(),
				client->getInet(), channel->getName(), "-", mode, nickName),
				it->second.client->getClientSocket());
		}
	}
}

void addModeChannel(Client *client, Server *server, Channel *channel, char mode)
{
	if (!channel->getModeStatus(mode)) {
		channel->addMode(mode);
		Channel::mapClients	clients = channel->getClients();
		if (mode == 'k' || mode == 'l') {
			std::string arg;
			if (mode == 'l') {
				std::stringstream limit;
				limit << channel->getClientLimit();
				arg = limit.str();
			}
			else
				arg = channel->getKey();
			for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
				server->sendClient(RPL_MODE_CHANNEL_PARAM(client->getNickName(), client->getUserName(),
					client->getInet(), channel->getName(), "+", mode, arg),
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

void notOperator(Client *client, Server *server, Channel *channel, bool half) {
	if (half) {
		server->sendClient(ERR_HALF_CHANOPRIVSNEEDED(client->getNickName(), channel->getName()), 
			client->getClientSocket());
	} else {
		server->sendClient(ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName()), 
			client->getClientSocket());
	}
}


void	printBannedList(Client *client, Server *server, Channel *channel)
{
	Channel::mapBan	banned = channel->getBanList();

	for (Channel::itMapBan it = banned.begin(); it != banned.end(); it++) {
		server->sendClient(RPL_BANLIST(client->getNickName(), channel->getName(), it->first,
			it->second.banBy, it->second.time), client->getClientSocket());
	}
	server->sendClient(RPL_ENDOFBANLIST(client->getNickName(), channel->getName()),
		client->getClientSocket());
}

void channelAddMode(Client *client, const Message &message, Server *server, Channel *channel, size_t *i, size_t *modeArg)
{
	size_t j = *i;
	(void)modeArg;
	while(j < message.getParameters()[1].size() && message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		case 'i': // i : set the channel to invite only
		case 's': // s : set the channel to secret
			if (channel->getOperGrade(client->getNickName()) == 3) // operator only
				addModeChannel(client, server, channel, message.getParameters()[1][j]);
			else
				notOperator(client, server, channel, false);
			break;
		case 'n': // n : set the channel to no external messages
		case 't': // t : only ops can change the topic
		case 'm': // m : only ops can send messages to the channel
			if (channel->getOperGrade(client->getNickName()) >= 2) // half-operator and +
				addModeChannel(client, server, channel, message.getParameters()[1][j]);
			else
				notOperator(client, server, channel, true);
			break;
		case 'k': // k : set the channel key
			if (channel->getOperGrade(client->getNickName()) >= 2) { // half-operator and +
				if (message.getParameters().size() > *modeArg && channel->getKey().empty()) { // required the password in argument
					channel->setKey(message.getParameters()[*modeArg]);			
					addModeChannel(client, server, channel, message.getParameters()[1][j]);
					(*modeArg)++;
				} 
			} else
				notOperator(client, server, channel, true);
			break;
		case 'l': // l : set the limit of users in the channel 
			if (channel->getOperGrade(client->getNickName()) == 3) { // operator only
				if (message.getParameters().size() > *modeArg
					&& message.getParameters()[*modeArg].find_first_not_of("0123456789")
					== std::string::npos) { // required the limit in argument
					channel->setClientLimit(message.getParameters()[*modeArg]);
					addModeChannel(client, server, channel, message.getParameters()[1][j]);
					(*modeArg)++;
				}
			} else
				notOperator(client, server, channel, false);
			break;
		case 'b': // b : user is banned from the channel (required the mask/user in argument)
			if (message.getParameters().size() > *modeArg
				&& channel->getOperGrade(client->getNickName()) >= 2) { 		
				if (channel->addBanned(toUpper(message.getParameters()[*modeArg]), client->getNickName())) {
					Channel::mapClients	clients = channel->getClients();
					for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
						server->sendClient(RPL_MODE_CHANNEL_PARAM(client->getNickName(), client->getUserName(),
							client->getInet(), channel->getName(), "+", "b", message.getParameters()[*modeArg]),
							it->second.client->getClientSocket());
					}
				}
				(*modeArg)++;
			} else if (message.getParameters().size() == *modeArg) { // if no argument, print the banned list
				printBannedList(client, server, channel);
			}
			break;
		case 'o': // o : give channel operator privileges to a user
			if (channel->getOperGrade(client->getNickName()) == 3) { // operator only
				if (message.getParameters().size() > *modeArg
					&& channel->isClientInChannel(message.getParameters()[*modeArg])) { // required the user in argument		
					addUserModeChannel(client, server, channel,
						message.getParameters()[1][j], message.getParameters()[*modeArg]);
					(*modeArg)++;
				}
			} else
				notOperator(client, server, channel, false);
			break;
		case 'h': // h : give channel half-operator privileges to a user
		case 'v': // v : give channel voice to a user
			if (channel->getOperGrade(client->getNickName()) >= 2) {// half-operator and +
				if (message.getParameters().size() > *modeArg
					&& channel->isClientInChannel(message.getParameters()[*modeArg])) { // required the user in argument		
					addUserModeChannel(client, server, channel,
						message.getParameters()[1][j], message.getParameters()[*modeArg]);
					(*modeArg)++;
				}
			} else
				notOperator(client, server, channel, false);
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

void channelRemoveMode(Client *client, const Message &message, Server *server, Channel *channel, size_t *i, size_t *modeArg)
{
	size_t j = *i + 1;
	(void)modeArg;
	while(j < message.getParameters()[1].size()
		&& message.getParameters()[1][j] != '-'
		&& message.getParameters()[1][j] != '+') {
		switch (message.getParameters()[1][j]) {
		case 'i': // i : remove the channel mode invite only 
		case 's': // s : remove the channel mode secret
		case 'l': // l : remove the channel mode limit of users in the channel
			if (channel->getOperGrade(client->getNickName()) == 3) // operator only
				removeModeChannel(client, server, channel, message.getParameters()[1][j]);
			else
				notOperator(client, server, channel, false);
			break;
		case 'n': // n : remove the channel mode no external messages
		case 't': // t : remove the channel mode that only ops can change the topic
		case 'm': // m : remove the channel mode that only ops can send messages to the channel
			if (channel->getOperGrade(client->getNickName()) >= 2) // half-operator and +
				removeModeChannel(client, server, channel, message.getParameters()[1][j]);
			else
				notOperator(client, server, channel, true);
			break;
		case 'k': // k : remove the channel key 
			if (channel->getOperGrade(client->getNickName()) >= 2) { // half-operator and +
				if (message.getParameters().size() > *modeArg) { // required the password defined in argument
					if (channel->getKey() == message.getParameters()[*modeArg]) {
						channel->setKey("");
						removeModeChannel(client, server, channel, message.getParameters()[1][j]);
					}
					(*modeArg)++;
				}
			} else
				notOperator(client, server, channel, true);
			break;
		case 'b': // b : user is banned from the channel (required the mask/user in argument)
			if (message.getParameters().size() > *modeArg
				&& channel->getOperGrade(client->getNickName()) >= 2) { // required the user in argument		
				if (channel->removeBanned(toUpper(message.getParameters()[*modeArg]))) {
					Channel::mapClients	clients = channel->getClients();
					for (Channel::itMapClients it = clients.begin(); it != clients.end(); it++) {
						server->sendClient(RPL_MODE_CHANNEL_PARAM(client->getNickName(), client->getUserName(),
							client->getInet(), channel->getName(), "-", "b", message.getParameters()[*modeArg]),
							it->second.client->getClientSocket());
					}
				}
				(*modeArg)++;
			} else if (message.getParameters().size() == *modeArg) { // if no argument, print the banned list
				printBannedList(client, server, channel);
			}
			break;
		case 'o': // o : remove the channel operator privileges to a user
			if (channel->getOperGrade(client->getNickName()) == 3) {// operator only
				if (message.getParameters().size() > *modeArg
					&& channel->isClientInChannel(message.getParameters()[*modeArg])) { // required the user in argument		
					removeUserModeChannel(client, server, channel, message.getParameters()[1][j],
						message.getParameters()[*modeArg]);
					(*modeArg)++;
				}
			} else
				notOperator(client, server, channel, false);
			break;
		case 'h': // h : remove the channel half-operator privileges to a user
		case 'v': // v : remove the channel voice to a user
			if (channel->getOperGrade(client->getNickName()) >= 2) {// half-operator and +
				if (message.getParameters().size() > *modeArg
					&& channel->isClientInChannel(message.getParameters()[*modeArg])) { // required the user in argument		
					removeUserModeChannel(client, server, channel, message.getParameters()[1][j],
						message.getParameters()[*modeArg]);
					(*modeArg)++;
				}
			} else
				notOperator(client, server, channel, false);
			break;
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

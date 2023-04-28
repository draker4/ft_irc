/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botHelp.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:42:02 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 19:27:27 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "botHelp.hpp"
#include "Server.hpp"

void	Client::sendHelp(Client *client, Server *server, const Message &message)
{
	if (!server || !client)
		return ;
	
	std::string	commands[] = {"INVITE", "JOIN", "KICK", "KILL", "LIST", "MODE",
		"MOTD", "NAMES", "NICK", "NOTICE", "OPER", "PART", "PASS", "PING",
		"PRIVMSG", "QUIT", "TOPIC", "USER", "WALLOPS", "WHO", "WHOIS"};
	size_t	array = sizeof(commands)/sizeof(commands[0]);
	
	size_t	i = 0;
	
	if (message.getParameters().size() <= 1) {
		std::string	list_commands = " Here is the list of all commands you can use in this server:";

		for (i = 0; i < array; i++) {
			list_commands.append(commands[i]);
			if (i != array - 1)
				list_commands.append(", ");
		}
		list_commands.append(". If you need the definition of a command, you can send the name of the command as parameter to your message to the helping bot, just like this : /PRIVMSG help <command>. Try with the JOIN command, it will be very helpful :)");
		
		server->sendClient(RPL_CMD(this->getNickName(), this->getUserName(),
			this->getInet(), "NOTICE", client->getNickName() + list_commands), client->getClientSocket());
		return ;
	}

	std::string	reply;

	for (i = 0; i < array; i++) {
		if (toUpper(message.getParameters()[1]) == commands[i]) {
			break ;
		}
	}

	switch (i) {
		case 0:
			reply = str_invite;
			break;
		case 1:
			reply = str_join;
			break;
		case 2:
			reply = str_kick;
			break;
		case 3:
			reply = str_kill;
			break;
		case 4:
			reply = str_list;
			break;
		case 5:
			reply = str_mode;
			break;
		case 6:
			reply = str_motd;
			break;
		case 7:
			reply = str_names;
			break;
		case 8:
			reply = str_nick;
			break;
		case 9:
			reply = str_notice;
			break;
		case 10:
			reply = str_oper;
			break;
		case 11:
			reply = str_part;
			break;
		case 12:
			reply = str_pass;
			break;
		case 13:
			reply = str_ping;
			break;
		case 14:
			reply = str_privmsg;
			break;
		case 15:
			reply = str_quit;
			break;
		case 16:
			reply = str_topic;
			break;
		case 17:
			reply = str_user;
			break;
		case 18:
			reply = str_wallops;
			break;
		case 19:
			reply = str_who;
			break;
		case 20:
			reply = str_whois;
			break;
		default :
			reply = " Command not found";
			break;
	}
	
	server->sendClient(RPL_CMD(this->getNickName(), this->getUserName(),
		this->getInet(), "NOTICE", client->getNickName() + reply), client->getClientSocket());
}

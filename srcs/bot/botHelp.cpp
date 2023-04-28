/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botHelp.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:42:02 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 18:15:10 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "botHelp.hpp"
#include "Server.hpp"

void	Client::sendHelp(Client *client, Server *server, std::string msg)
{
	if (msg.empty() || !server || !client)
		return ;
	
	std::string	commands[] = {"INVITE", "JOIN", "KICK", "KILL", "LIST", "MODE",
		"MOTD", "NAMES", "NICK", "NOTICE", "OPER", "PART", "PASS", "PING",
		"PRIVMSG", "QUIT", "TOPIC", "USER", "WALLOPS", "WHO", "WHOIS"};
	
	std::string	reply;
	size_t	i = 0;
	size_t	array = sizeof(commands)/sizeof(commands[0]);

	for (i = 0; i < array; i++) {
		std::cout << "LAAA i=" << i << " et command=" << commands[i] << std::endl;
		if (toUpper(msg) == commands[i]) {
			std::cout << "HERE=" << msg << std::endl;
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

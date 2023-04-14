/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/13 13:09:41 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The PING command is sent to check the other side of the connection is still
 * 		connected, and/or to check for connection latency.
 * 
 * 		When receiving a PING message, clients or servers must reply to it with a
 * 		PONG message with the same <token> value. 
 * 
 * 		Numeric replies:
 * 		461 - ERR_NEEDMOREPARAMS -  "<client> <command> :Not enough parameters" > DEALT BY IRSSI
 * 		409 - ERR_NOORIGIN - "<client> :No origin specified" (when <token> is empty) > DEALT BY IRSSI
 * 
 * 		IRSSI :
 * 		With this reference client, sending a PING to a server sends a CTCP request
 * 		to a nickname or a channel. Using the char '*' pings every user in a channel.
 * 		Syntax : PING [<nick> | <channel> | *]
 * 
 * @return SUCCESS (0) or FAILURE (1) 
 */
void ping(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "PING command called" << RESET << std::endl;
	
	if (message.getParameters().empty())
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("PING")), 
			client->getClientSocket());
	else
	{
		// regler with real timestamp !!
		timeval	tm;
		gettimeofday(&tm, NULL);
		double	time = (tm.tv_sec - 50000) * 1000 + tm.tv_usec / 1000 - strtod(message.getParameters()[0].c_str(), NULL);
		// std::cout << BLUE << std::fixed << time << std::endl;
		std::stringstream	ss;
		ss << std::fixed << strtod(message.getParameters()[0].c_str(), NULL) - time;
		std::cout << RED << std::string(ss.str()) << std::endl;

		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(), client->getInet(),
		std::string("PONG"), SERVERNAME +std::string(" ") + std::string(ss.str())),
			client->getClientSocket());
	}
}

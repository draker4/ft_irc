/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/28 15:53:22 by bperriol         ###   ########lyon.fr   */
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
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(), client->getInet(),
		std::string("PONG"), SERVERNAME +std::string(" ") + message.getParameters()[0]),
			client->getClientSocket());
	}
}

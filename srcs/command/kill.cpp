/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/13 16:25:32 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The KILL command is used to close the connection between a given client
 * 		and the server they are connected to. KILL is a privileged command and 
 * 		is available only to IRC Operators. Clients can rejoin instantly after 
 * 		this command is performed on them.
 * 	
 * 	Syntax :
 * 	KILL <nickname> <comment>
 * 
 * 		<nickname> represents the user to be ‘killed’, and 
 * 		<comment> is shown to all users and to the user themselves upon being killed.
 * 
 * 		The <source> of the message should be the operator who performed the command.
 */
void kill(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "KILL command called" << RESET << std::endl;
	
	if (!client->getMode('o'))
	{
		server->sendClient(ERR_NOPRIVILEGES(client->getNickname()), client->getClientSocket());
		return ;
	}
	// else if (!client.getMode('find mode'))
	// 	server->sendClient(ERR_NOPRIVS);
	else if (message.getParameters().size() < 2)
	{
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("KILL")), 
			client->getClientSocket());
		return ;
	}
	Client	*to_kill = server->getClient(message.getParameters()[0]);
	if (!to_kill)
		server->sendClient(ERR_NOSUCHNICK(client->getNickname(), message.getParameters()[0]),
			client->getClientSocket());
	else {
		// send kill reply to clietn being killed
		server->sendClient(RPL_CMD(client->getNickname(), to_kill->getUsername(), to_kill->getInet(),
		std::string("KILL"), std::string("")), to_kill->getClientSocket());
		
		// send quit message to all users in channel
		//for ()
	}
}

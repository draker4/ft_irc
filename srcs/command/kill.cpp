/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:21 by baptiste         ###   ########lyon.fr   */
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
	(void)client;
	(void)message;
	(void)server;
}

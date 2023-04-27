/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/27 17:47:48 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The QUIT command is used to terminate a client’s connection to the server. 
 *  The server acknowledges this by replying with an ERROR message and closing 
 *  the connection to the client.
 * 
 * 	When connections are terminated by a client-sent QUIT command, 
 *  servers SHOULD prepend <reason> with the ASCII string "Quit: " when sending 
 *  QUIT messages to other clients, to represent that this user terminated the 
 *  connection themselves. This applies even if <reason> is empty, in which case 
 *  the reason sent to other clients SHOULD be just this "Quit: " string. 
 * 	
 *  Parameters: [<reason>]
 * 
 *  Example : [CLIENT] QUIT :Gone to have lunch   
 * 			  [SERV] ;Client exiting from the network
 * 
 * 	Source: https://modern.ircdocs.horse/#quit-message
 */
void quit(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "QUIT command called" << RESET << std::endl;
	
	// get reason
	std::string	reason = " ";
	if (message.getParameters().size() > 0) {
		reason.append(message.getParameters()[0]);
		client->setReasonLeaving(reason);
	}
	
	// send ERROR message to client
	server->sendClient(ERROR_MESSAGE(reason), client->getClientSocket());
	
	// delete client from server
	client->setDeconnect(true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:58 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief Command : TOPIC <channel> [<topic>]
 * 			
 * 	The TOPIC command is used to change or view the topic of the given channel. 
 * 	If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned 
 * 	specifying the current channel topic or lack of one. 
 * 	If <topic> is an empty string, the topic for the channel will be cleared.
 * 
 * 	Numeric Replies:
 * 	
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_NOSUCHCHANNEL (403)
 * 	ERR_NOTONCHANNEL (442)
 * 	RPL_NOTOPIC (331)
 * 	RPL_TOPIC (332)
 * 	
 * 	Examples:
 * 	[CLIENT] TOPIC #test :New topic
 *  [SERVER] ; Setting the topic on "#test" to "New topic".
 * 
 * 	[CLIENT] TOPIC #test :
 * 	[SERVER] ; Clearing the topic on "#test"
 * 
 * 	[CLIENT] TOPIC #test
 * 	[SERVER] ; Checking the topic for "#test"
 * 
 */
void topic(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "TOPIC command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

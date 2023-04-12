/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:23 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief If the exact name of a channel is given, the only information about 
 * 		this channel is requested; otherwise, a list of all channels will be 
 * 		displayed.
 * 
 * 	Syntax : 
 * 	[IRSSI] LIST [-yes] [<channel>]
 * 	
 * 	Numeric Replies:
 * 		RPL_LISTSTART (321) : marks the start of a channel list. 
 * 		RPL_LIST (322) : sends information about a channel to the client.
 * 		RPL_LISTEND (323) : indicates the end of a LIST response.
 * 
 * 	Examples:
 * 		/LIST
 * 		/LIST -yes => "LIST" when received by server
 * 		/LIST #ubuntu
 */
void list(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "LIST command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

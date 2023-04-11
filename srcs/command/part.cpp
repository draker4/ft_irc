/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:43 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The PART command removes the client from the given channel(s).
 * 
 * 	Syntax: PART <channel>{,<channel>} [<reason>]
 * 
 *  On sending a successful PART command, the user will receive a PART message 
 *  from the server for each channel they have been removed from. 
 *  <reason> is the reason that the client has left the channel(s).
 * 
 *  Numeric Replies:
 *   ERR_NEEDMOREPARAMS (461)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_NOTONCHANNEL (442)
 * 	
 * 	Example:
 * 	[IRSSI] /PART #test,#hey :Dining
 * 	[SERVER] leaves both channels #test and #hey with the reason "Dining"
 * 	[SERVER to CLIENT]"@user_id PART #channel Dining" (for EACH channel they leave)
 */
void part(Client *client, const Message &message, Server *server)
{
	std::cout << BLUE << "PART command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

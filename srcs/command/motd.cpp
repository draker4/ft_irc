/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:28 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The MOTD command is used to get the “Message of the Day” of the given 
 * server. If <target> is not given, the MOTD of the server the client is 
 * connected to should be returned.
 * 
 *  Syntax : MOTD [<target>]
 * 
 *  If the MOTD can be found, one RPL_MOTDSTART numeric is returned, followed by
 *  one or more RPL_MOTD numeric, then one RPL_ENDOFMOTD numeric.
 * 
 * 	If the MOTD does not exist or could not be found, the ERR_NOMOTD numeric is returned.
 * 
 *  Numeric replies:
 *   ERR_NOSUCHSERVER (402)
 *   ERR_NOMOTD (422)
 *   RPL_MOTDSTART (375)
 *   RPL_MOTD (372)
 *   RPL_ENDOFMOTD (376)
 */
void motd(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "MOTD command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

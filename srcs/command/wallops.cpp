/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 18:34:38 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 20:05:08 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The WALLOPS command is used to send a message to all currently connected users who have set the ‘w’ user mode for themselves. The <text> SHOULD be non-empty.
 * 	Servers MAY echo WALLOPS messages to their sender even if they don’t have the ‘w’ user mode.
 * 	Servers MAY send WALLOPS only to operators.
 * 	Servers may generate it themselves, and MAY allow operators to send them.
 * 	
 * 	Numeric replies:
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_NOPRIVILEGES (481)
 * 	ERR_NOPRIVS (723)
 * 
 * 	Syntax : WALLOPS <text>
 */
void wallops(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "INVITE command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

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

// The KILL command is used to close the connection between a given client and
// the server they are connected to. KILL is a privileged command and is available
// only to IRC Operators.
void kill(Client *client, const Message &message, Server *server)
{
	std::cout << BLUE << "KILL command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

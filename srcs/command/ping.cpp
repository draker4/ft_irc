/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:49 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The PING command is sent by either clients or servers to check the other side of
// the connection is still connected and/or to check for connection latency,
// at the application layer.
void ping(Client *client, const Message &message, Server *server)
{
	std::cout << BLUE << "PING command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

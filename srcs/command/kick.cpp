/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:02:13 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The KICK command can be used to request the forced removal of a user from a channel.
void kick(const int &clientSocket, const Message &message, Server *server)
{
	// std::cout << BLUE << "KICK command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

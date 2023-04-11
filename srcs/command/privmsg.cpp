/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 15:38:41 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The PRIVMSG command is used to send private messages between users, as well as to send messages to channels.
void privmsg(const int &clientSocket, Message *message, Server *server)
{
	// std::cout << BLUE << "PRIVMSG command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

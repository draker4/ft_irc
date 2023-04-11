/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:12 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The INVITE command is used to invite a user to a channel.
void invite(const int &clientSocket, const Message &message, Server *server)
{
	std::cout << BLUE << "INVITE command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

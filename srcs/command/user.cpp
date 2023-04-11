/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:31:02 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The USER command is used at the beginning of a connection to specify the username
// and realname of a new user.
void user(const int &clientSocket, const Message &message, Server *server)
{
	std::cout << BLUE << "USER command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

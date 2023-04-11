/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 15:38:41 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The QUIT command is used to terminate a client’s connection to the server.
// The server acknowledges this by replying with an ERROR message and closing
// the connection to the client.
void quit(const int &clientSocket, Message *message, Server *server)
{
	// std::cout << BLUE << "QUIT command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

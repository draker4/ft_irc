/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 15:38:41 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The LIST command is used to get a list of channels along with some information about each channel.
void list(const int &clientSocket, Message *message, Server *server)
{
	// std::cout << BLUE << "LIST command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

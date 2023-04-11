/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 15:38:41 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The TOPIC command is used to change or view the topic of the given channel.
void topic(const int &clientSocket, Message *message, Server *server)
{
	// std::cout << BLUE << "TOPIC command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

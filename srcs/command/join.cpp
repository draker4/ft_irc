/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:15 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The JOIN command indicates that the client wants to join the given channel(s),
// each channel using the given key for it.
void join(const int &clientSocket, const Message &message, Server *server)
{
	std::cout << BLUE << "JOIN command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

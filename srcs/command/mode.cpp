/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:02:13 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The MODE command is used to set or remove options (or modes) from a given target.
// Our user modes : i, o. Our channels modes: b,k,m,o,p,s,t,v
void mode(const int &clientSocket, const Message &message, Server *server)
{
	// std::cout << BLUE << "MODE command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

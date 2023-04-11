/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:43 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The PART command removes the client from the given channel(s).
void part(const int &clientSocket, const Message &message, Server *server)
{
	std::cout << BLUE << "PART command called" << RESET << std::endl;
	(void)clientSocket;
	(void)message;
	(void)server;
}

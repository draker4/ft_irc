/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:09 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The CAP command is used to
 * 	
 * 	Syntax :
 * 	CAP <...>
 * 
 * 		<...> represents ...
 */
void cap(Client *client, const Message &message, Server *server)
{
	std::cout << BLUE << "CAP command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

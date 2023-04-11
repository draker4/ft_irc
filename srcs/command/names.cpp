/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:31 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

// The NAMES command is used to view the nicknames joined to a channel and their channel
// membership prefixes.
void names(Client *client, const Message &message, Server *server)
{
	std::cout << BLUE << "NAMES command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}

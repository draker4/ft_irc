/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 18:34:38 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 18:35:30 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

void join(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "JOIN command called" << RESET << std::endl;
	
	if (message.getParameters().empty()) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("WHO")), 
			client->getClientSocket());
	}
}

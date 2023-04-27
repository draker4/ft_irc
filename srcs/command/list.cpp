/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:23 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief If the exact name of a channel is given, the only information about 
 * 		this channel is requested; otherwise, a list of all channels will be 
 * 		displayed.
 * 
 * 	Syntax : 
 * 	[IRSSI] LIST [-yes] [<channel>]
 * 	
 * 	Numeric Replies:
 * 		RPL_LISTSTART (321) : marks the start of a channel list. 
 * 		RPL_LIST (322) : sends information about a channel to the client.
 * 		RPL_LISTEND (323) : indicates the end of a LIST response.
 * 
 * 	Examples:
 * 		/LIST
 * 		/LIST -yes => "LIST" when received by server
 * 		/LIST #ubuntu
 */
void list(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "LIST command called" << RESET << std::endl;
	(void)message;
	Server::vecChannel channels = server->getChannels();
	for (Server::itVecChannel it = channels.begin(); it != channels.end(); it++) {
		size_t size = (*it)->getClients().size();
		if (size > 0 && !(*it)->getModeStatus('s')) {
			std::stringstream ss;
			ss << size;
			server->sendClient(RPL_LIST(client->getNickName(), (*it)->getName(), ss.str(),
				(*it)->getTopic()), client->getClientSocket());
		}
	}
	server->sendClient(RPL_LISTEND(client->getNickName()), client->getClientSocket());
}

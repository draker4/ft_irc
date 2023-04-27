/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:28 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The MOTD command is used to get the “Message of the Day” of the given 
 * server. If <target> is not given, the MOTD of the server the client is 
 * connected to should be returned.
 * 
 *  If the MOTD can be found, one RPL_MOTDSTART numeric is returned, followed by
 *  one or more RPL_MOTD numeric, then one RPL_ENDOFMOTD numeric.
 * 
 * 	If the MOTD does not exist or could not be found, the ERR_NOMOTD numeric is returned.
 * 
 *  Numeric replies:
 *   ERR_NOMOTD (422)
 *   RPL_MOTDSTART (375)
 *   RPL_MOTD (372)
 *   RPL_ENDOFMOTD (376)
 * 
 * 	Syntax : MOTD
 */
void motd(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "MOTD command called" << RESET << std::endl;
	(void)message;
	std::ifstream	file;
	std::string		content;

	file.open("config/motd.txt");
	if (!file.is_open()) {
		std::cout << "file not open" << std::endl;
		server->sendClient(ERR_NOMOTD(client->getNickName()), client->getClientSocket());
		return;
	}
	std::getline( file, content, '\0');
	if (content.empty()) {
		std::cout << "content empty" << std::endl;
		server->sendClient(ERR_NOMOTD(client->getNickName()), client->getClientSocket());
	}
	else {
		std::cout << "content: " << content << std::endl;
		server->sendClient(RPL_MOTDSTART(client->getNickName()), client->getClientSocket());
		server->sendClient(RPL_MOTD(client->getNickName(), content), client->getClientSocket());
		server->sendClient(RPL_ENDOFMOTD(client->getNickName()), client->getClientSocket());
	}
	file.close();
}

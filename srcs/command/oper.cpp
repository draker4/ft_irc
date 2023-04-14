/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:40 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The OPER command is used by a normal user to obtain IRC operator privileges.
 * 	Both parameters are required for the command to be successful.
 *
 *  Command: OPER
 *  Parameters: <name> <password>
 *
 * Numeric Replies:
 *
 * ERR_NEEDMOREPARAMS (461)
 * ERR_PASSWDMISMATCH (464)
 * ERR_NOOPERHOST (491)
 * RPL_YOUREOPER (381)
 *
 * Example:
 *  [CLIENT] OPER foo bar
 *  [SERVER] ; Attempt to register as an operator using a name of "foo" and the password "bar".
 */
void oper(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "OPER command called" << RESET << std::endl;
	if (message.getParameters().size() < 2) {
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), message.getCommand()),
			client->getClientSocket());
	} else {
		bool					rejectHost = false;	
		std::string 			name = message.getParameters()[0];
		std::string				password = message.getParameters()[1];
		Server::vecOpeConfig	opeConf = server->getOpeConf();
		for (Server::itVecOpeConfig itOpeConf = opeConf.begin();
			itOpeConf != opeConf.end(); itOpeConf++) {
			if (itOpeConf->name == name && itOpeConf->password == password) {
				if (itOpeConf->host == client->getInet()) {
					client->addMode('o');
					server->sendClient(RPL_YOUREOPER(client->getNickName()),
						client->getClientSocket());
					return ;
				} else {
					rejectHost = true;
				}
				
			}
		}
		if (rejectHost) {
			server->sendClient(ERR_NOOPERHOST(client->getNickName()),
				client->getClientSocket());
		} else {
			server->sendClient(ERR_PASSWDMISMATCH(client->getNickName()),
				client->getClientSocket());
		}
	}
}

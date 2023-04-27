/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/27 12:27:29 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "errors.hpp"
#include "replies.hpp"
#include "Message.hpp"

/**
 * @brief The KILL command is used to close the connection between a given client
 * 		and the server they are connected to. KILL is a privileged command and 
 * 		is available only to IRC Operators. Clients can rejoin instantly after 
 * 		this command is performed on them.
 * 	
 * 	Syntax :
 * 	KILL <nickname> <comment>
 * 
 * 		<nickname> represents the user to be ‘killed’, and 
 * 		<comment> is shown to all users and to the user themselves upon being killed.
 * 
 * 		The <source> of the message should be the operator who performed the command.
 */
void kill(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "KILL command called" << RESET << std::endl;
	
	if (!client->getModeStatus('o'))
	{
		server->sendClient(ERR_NOPRIVILEGES(client->getNickName()), client->getClientSocket());
		return ;
	}
	else if (message.getParameters().size() < 2)
	{
		server->sendClient(ERR_NEEDMOREPARAMS(client->getNickName(), std::string("KILL")), 
			client->getClientSocket());
		return ;
	}
	Client	*to_kill = server->getClient(message.getParameters()[0]);
	if (!to_kill)
		server->sendClient(ERR_NOSUCHNICK(client->getNickName(), message.getParameters()[0]),
			client->getClientSocket());
	
	// SUCCESS
	else {
		
		// send error msg to client being killed
		std::string	rpl_error = "Closing link: " + SERVERNAME + " Killed " + to_kill->getNickName() + " because " +
			message.getParameters()[1];
		server->sendClient(ERROR_MESSAGE(rpl_error), to_kill->getClientSocket());
		
		// send kill reply to client being killed
		std::string	rpl_kill = to_kill->getNickName() + " " + message.getParameters()[1];
		server->sendClient(RPL_CMD(client->getNickName(), client->getUserName(), client->getInet(),
		std::string("KILL"), rpl_kill), to_kill->getClientSocket());
		
		// reason of the QUIT message to send
		// std::string	rpl_quit = ":Killed by " + client->getUserName() + " because " + message.getParameters()[1];
		
		// find all channels the client was in
		Client::vecChannel	channels = to_kill->getChannels();
		std::cerr << PURPLE << " In kill function nb channels: " << channels.size() << RESET << std::endl;
	
		for (Client::itVecChannel it = channels.begin(); it != channels.end(); it++) {
			
			// remove client from the channel
			std::cerr << PURPLE << " In for loop in kill nb clients: " << (*it)->getClients().size() << RESET << std::endl;
			// (*it)->removeClient(client);
			
			// if the channel is empty, delete it
			// if ((*it)->getClients().empty())
			// 	server->removeChannel(*it);
			
			// get all clients from channel
			// Channel::mapClients	clients = (*it)->getClients();
			
			// send QUIT message
			// for (Channel::itMapClients it_client = clients.begin(); it_client != clients.end(); it_client++) {
			// 	server->sendClient(RPL_CMD(to_kill->getNickName(), to_kill->getUserName(), to_kill->getInet(),
			// 		std::string("QUIT"), rpl_quit), it_client->second.client->getClientSocket());
			// }
		}
		
		// deconnect client
		if (to_kill->getNickName() == client->getNickName())
			to_kill->setDeconnect(true);
		else
			server->deleteClient(to_kill->getClientSocket());
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:14:17 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:31:14 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Server.hpp"

class Server;

void	cap(const int &clientSocket, const Message &message, Server *server);
void	invite(const int &clientSocket, const Message &message, Server *server);
void	join(const int &clientSocket, const Message &message, Server *server);
void	kick(const int &clientSocket, const Message &message, Server *server);
void	kill(const int &clientSocket, const Message &message, Server *server);
void	list(const int &clientSocket, const Message &message, Server *server);
void	mode(const int &clientSocket, const Message &message, Server *server);
void	motd(const int &clientSocket, const Message &message, Server *server);
void	names(const int &clientSocket, const Message &message, Server *server);
void	nick(const int &clientSocket, const Message &message, Server *server);
void	notice(const int &clientSocket, const Message &message, Server *server);
void	oper(const int &clientSocket, const Message &message, Server *server);
void	part(const int &clientSocket, const Message &message, Server *server);
void	pass(const int &clientSocket, const Message &message, Server *server);
void	ping(const int &clientSocket, const Message &message, Server *server);
void	privmsg(const int &clientSocket, const Message &message, Server *server);
void	quit(const int &clientSocket, const Message &message, Server *server);
void	topic(const int &clientSocket, const Message &message, Server *server);
void	user(const int &clientSocket, const Message &message, Server *server);

#endif

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

void	cap(Client *client, const Message &message, Server *server);
void	invite(Client *client, const Message &message, Server *server);
void	join(Client *client, const Message &message, Server *server);
void	kick(Client *client, const Message &message, Server *server);
void	kill(Client *client, const Message &message, Server *server);
void	list(Client *client, const Message &message, Server *server);
void	mode(Client *client, const Message &message, Server *server);
void	motd(Client *client, const Message &message, Server *server);
void	names(Client *client, const Message &message, Server *server);
void	nick(Client *client, const Message &message, Server *server);
void	notice(Client *client, const Message &message, Server *server);
void	oper(Client *client, const Message &message, Server *server);
void	part(Client *client, const Message &message, Server *server);
void	pass(Client *client, const Message &message, Server *server);
void	ping(Client *client, const Message &message, Server *server);
void	privmsg(Client *client, const Message &message, Server *server);
void	quit(Client *client, const Message &message, Server *server);
void	topic(Client *client, const Message &message, Server *server);
void	user(Client *client, const Message &message, Server *server);

#endif

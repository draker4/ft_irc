/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:14:17 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/24 19:12:23 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Server.hpp"
# include <set>

# define DEBUG_COMMAND true

class Server;

// prototype of functions written in commands.cpp
Message::vecString	split(std::string str, std::string c);

// The CAP command is used to 
void	cap(Client *client, const Message &message, Server *server);

// The INVITE command is used to invite a user to a channel.
void	invite(Client *client, const Message &message, Server *server);

// The JOIN command indicates that the client wants to join the given channel(s),
// each channel using the given key for it.
void	join(Client *client, const Message &message, Server *server);

// The KICK command can be used to request the forced removal of a user from a channel.
void	kick(Client *client, const Message &message, Server *server);

// The KILL command is used to close the connection between a given client and
// the server they are connected to. KILL is a privileged command and is available
// only to IRC Operators.
void	kill(Client *client, const Message &message, Server *server);

// The LIST command is used to get a list of channels along with some information about each channel.
void	list(Client *client, const Message &message, Server *server);

// The MODE command is used to set or remove options (or modes) from a given target.
// Our user modes : i, o. Our channels modes: b,k,m,o,p,s,t,v
void	mode(Client *client, const Message &message, Server *server);

// The MOTD command is used to get the “Message of the Day” of the given server.
void	motd(Client *client, const Message &message, Server *server);

// The NAMES command is used to view the nicknames joined to a channel and their channel
// membership prefixes.
void	names(Client *client, const Message &message, Server *server);

// The NICK command is used to give the client a nickname or change the previous one.
void	nick(Client *client, const Message &message, Server *server);

// The NOTICE command is used to send notices between users, as well as to send notices
// to channels. The difference between NOTICE and PRIVMSG is that automatic replies must
// never be sent in response to a NOTICE message.
void	notice(Client *client, const Message &message, Server *server);

// The OPER command is used by a normal user to obtain IRC operator privileges.
void	oper(Client *client, const Message &message, Server *server);

// The PART command removes the client from the given channel(s).
void	part(Client *client, const Message &message, Server *server);

// The PASS command is used to set a ‘connection password’. If set, the password
// must be set before any attempt to register the connection is made.
void	pass(Client *client, const Message &message, Server *server);

// The PING command is sent by either clients or servers to check the other side of
// the connection is still connected and/or to check for connection latency,
// at the application layer.
void	ping(Client *client, const Message &message, Server *server);

// The PRIVMSG command is used to send private messages between users, as well as to send messages to channels.
void	privmsg(Client *client, const Message &message, Server *server);

// The QUIT command is used to terminate a client’s connection to the server.
// The server acknowledges this by replying with an ERROR message and closing
// the connection to the client.
void	quit(Client *client, const Message &message, Server *server);

// The TOPIC command is used to change or view the topic of the given channel.
void	topic(Client *client, const Message &message, Server *server);

// The USER command is used at the beginning of a connection to specify the username
// and realname of a new user.
void	user(Client *client, const Message &message, Server *server);

// This command is used to query a list of users who match the provided mask.
void	who(Client *client, const Message &message, Server *server);

#endif

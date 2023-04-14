/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:25:35 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 14:52:04 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "Server.hpp"

// 401 - KILL - MODE
# define ERR_NOSUCHNICK(client, nickname) ( ":" + SERVERNAME + " 401 " + client + " " \
		+ nickname + " :No such nick/channel\r\n")

// 403 - JOIN - MODE
# define ERR_NOSUCHCHANNEL(client, channel) ( ":" + SERVERNAME + " 403 " + client + " " \
		+ channel + " :No such channel\r\n")

// 405 - JOIN
# define ERR_TOOMANYCHANNELS(client, channel) ( ":" + SERVERNAME + " 405 " + client + " " \
		+ channel + " :You have joined too many channels\r\n")

// 421 - Server::_handleCommand
# define ERR_UNKNOWNCOMMAND(client, command) ( ":" + SERVERNAME + " 421 " + client + " " \
		+ command + " :Unknown command\r\n")

// 431 - NICK
# define ERR_NONICKNAMEGIVEN ":" + SERVERNAME + " 431 :No nickname given\r\n"

// 432 - NICK
# define ERR_ERRONEUSNICKNAME(nickname) ( ":" + SERVERNAME + " 432 " + nickname \
		+ " :Erroneus nickname\r\n")

// 433 - NICK
# define ERR_NICKNAMEINUSE(nickname) ( ":" + SERVERNAME + " 433 " + nickname \
		+ " :Nickname is already in use\r\n")

// 461 - PASS - USER - OPER - MODE
# define ERR_NEEDMOREPARAMS(client, command) (":" +  SERVERNAME + " 461 " + client + " " \
		+ command + " :Not Enough parameters\r\n")

// 462 - PASS
# define ERR_ALREADYREGISTERED(client) ( ":" + SERVERNAME + " 462 " + client \
		+ " :You may not reregister\r\n")

// 464 - PASS - OPER
# define ERR_PASSWDMISMATCH(client) ( ":" + SERVERNAME + " 464 " + client \
		+ " :Password incorrect\r\n")

// 481 - KILL
# define ERR_NOPRIVILEGES(client) ( ":" + SERVERNAME + " 481 " + client \
		+ " :Permission Denied- You're not an IRC operator\r\n")

// 491 - OPER
# define ERR_NOOPERHOST(client) ( ":" + SERVERNAME + " 491 " + client \
		+ " :No O-lines for your host\r\n")

// 723 - KILL
// # define ERR_NOPRIVS(client, priv) ( ":" + SERVERNAME + " 723 " + client + " "
// 		+ priv + " :Insufficient oper privileges.\r\n")

// ERROR Message - PASS - KILL
# define ERROR_MESSAGE(reason) ("ERROR :" + reason + "\r\n")

#endif

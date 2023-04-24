/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:25:35 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/24 12:24:35 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "Server.hpp"

// 401 - KILL - MODE
# define ERR_NOSUCHNICK(client, nickname) ( ":" + SERVERNAME + " 401 " + client + " " \
		+ nickname + " :No such nick/channel\r\n")

// 403 - JOIN - MODE - TOPIC
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

// 442 - TOPIC
# define ERR_NOTONCHANNEL(nickname, channel) ( ":" + SERVERNAME + " 442 " + nickname + " " \
		+ channel + " :You're not on that channel\r\n")

// 461 - PASS - USER - OPER - MODE - TOPIC
# define ERR_NEEDMOREPARAMS(client, command) (":" +  SERVERNAME + " 461 " + client + " " \
		+ command + " :Not Enough parameters\r\n")

// 462 - PASS
# define ERR_ALREADYREGISTERED(client) ( ":" + SERVERNAME + " 462 " + client \
		+ " :You may not reregister\r\n")

// 464 - PASS - OPER
# define ERR_PASSWDMISMATCH(client) ( ":" + SERVERNAME + " 464 " + client \
		+ " :Password incorrect\r\n")

// 471 - JOIN
# define ERR_CHANNELISFULL(client, channel) ( ":" + SERVERNAME + " 471 " + client \
		+ " " + channel + " :Cannot join channel (+l)\r\n")

// 473 - JOIN
# define ERR_INVITEONLYCHAN(client, channel) ( ":" + SERVERNAME + " 473 " + client \
		+ " " + channel + " :Cannot join channel (+i)\r\n")

// 474 - JOIN
# define ERR_BANNEDFROMCHAN(client, channel) ( ":" + SERVERNAME + " 474 " + client \
		+ " " + channel + " :Cannot join channel (+b)\r\n")

// 475 - JOIN
# define ERR_BADCHANNELKEY(client, channel) ( ":" + SERVERNAME + " 475 " + client \
		+ " " + channel + " :Cannot join channel (+k)\r\n")

// 481 - KILL
# define ERR_NOPRIVILEGES(client) ( ":" + SERVERNAME + " 481 " + client \
		+ " :Permission Denied- You're not an IRC operator\r\n")

// 482 - MODE
# define ERR_CHANOPRIVSNEEDED(client, channel) ( ":" + SERVERNAME + " 482 " + client \
		+ " " + channel + " :You're not channel operator\r\n")


// 491 - OPER
# define ERR_NOOPERHOST(client) ( ":" + SERVERNAME + " 491 " + client \
		+ " :No O-lines for your host\r\n")

// 501 - MODE
# define ERR_UMODEUNKNOWNFLAG(client) ( ":" + SERVERNAME + " 501 " + client \
		+ " :Unknown MODE flag\r\n")

// 502 - MODE
# define ERR_USERSDONTMATCH(client) ( ":" + SERVERNAME + " 502 " + client \
		+ " :Cant change mode for other users\r\n")

// 723 - KILL
// # define ERR_NOPRIVS(client, priv) ( ":" + SERVERNAME + " 723 " + client + " "
// 		+ priv + " :Insufficient oper privileges.\r\n")

// ERROR Message - PASS - KILL
# define ERROR_MESSAGE(reason) ("ERROR :" + reason + "\r\n")

#endif

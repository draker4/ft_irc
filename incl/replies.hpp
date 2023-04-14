/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:04:29 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 17:15:01 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "Server.hpp"

// 001 - Server::sendWelcome
# define RPL_WELCOME(nickName, user, host) ( ":" + SERVERNAME + " 001 " + nickName \
		+ " :Welcome to the " + SERVERNAME + " Network, " + nickName + "!" + user + "@" + host + "\r\n")

// 002 - Server::sendWelcome
# define RPL_YOURHOST(nickName) ( ":" + SERVERNAME + " 002 " + nickName \
		+ " :Your host is " + SERVERNAME + ", running version " + VERSION + "\r\n")

// 003 - Server::sendWelcome
# define RPL_CREATED(nickName, datetime) ( ":" + SERVERNAME + " 003 " + nickName \
		+ " :This server was created " + datetime + "\r\n")

// 221 - MODE
# define RPL_UMODEIS(nickName, userModes) ( ":" + SERVERNAME + " 221 " + nickName \
		+ " :+" + userModes + "\r\n")

// 332 - JOIN
# define RPL_TOPIC(nickName, channel, topic) ( ":" + SERVERNAME + " 332 " + nickName \
		+ " " + channel +" :" + topic + "\r\n")

// 353 - JOIN
# define RPL_NAMREPLY(nickName, symbol, channel, prefix) ( ":" + SERVERNAME + " 353 " + nickName \
		+ " " + symbol + " " + channel + " :" + prefix + nickName + "\r\n")

// 366 - JOIN
# define RPL_ENDOFNAMES(nickName, channel) ( ":" + SERVERNAME + " 366 " + nickName \
		+ " " + channel + " :End of /NAMES list\r\n")

// 381 - OPER
# define RPL_YOUREOPER(nickName) ( ":" + SERVERNAME + " 381 " + nickName \
		+ " :You are now an IRC operator\r\n")

// SPE_CODE - KILL - PING - JOIN
# define RPL_CMD(nickname, user, host, cmd, reply) ( ":" + nickname + "!" + user + "@" + host + " " \
	+ cmd + " " + reply + "\r\n") 

// SPE_CODE - NICK
# define RPL_NICK(oldNickname, newNickname, user, host) ( ":" + oldNickname + "!" + user + "@" + host + " NICK " \
	+ newNickname + "\r\n") 

// SPE_CODE - MODE
# define RPL_MODE(nickname, user, host, sign, mode) ( ":" + nickname + "!" + user + "@" + host + " MODE " \
	+ nickname + " " + sign + mode + "\r\n") 

#endif

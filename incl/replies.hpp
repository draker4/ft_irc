/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:04:29 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/12 14:04:02 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "Server.hpp"

# define RPL_WELCOME(nickName, user, host) ( ":" + SERVERNAME + " 001 " + nickName \
		+ " :Welcome to the " + SERVERNAME + " Network, " + nickName + "!" + user + "@" + host + "\r\n") // 001 - Server::sendWelcome

# define RPL_YOURHOST(nickName) ( ":" + SERVERNAME + " 002 " + nickName \
		+ " :Your host is " + SERVERNAME + ", running version " + VERSION + "\r\n") // 002 - Server::sendWelcome

# define RPL_CREATED(nickName, datetime) ( ":" + SERVERNAME + " 003 " + nickName \
		+ " :This server was created " + datetime + "\r\n") // 003 - Server::sendWelcome

# define RPL_NICK(oldNickname, newNickname, user, host) ( ":" + oldNickname + "!" + user + "@" + host + " NICK " \
	+ newNickname + "\r\n") // SPE_CODE - NICK


#endif

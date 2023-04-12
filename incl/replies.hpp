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

# define RPL_WELCOME(client, host) ( ":" + SERVERNAME + " 001 " + client + " " \
		+ ":Welcome to the " + SERVERNAME + " Network, " + client + "!" + client + "@" + host + "\r\n") // 001 - Server::sendWelcome

# define RPL_YOURHOST(client) ( ":" + SERVERNAME + " 002 " + client + " " \
		+ ":Your host is " + SERVERNAME + ", running version " + VERSION + "\r\n") // 002 - Server::sendWelcome

# define RPL_CREATED(client, datetime) ( ":" + SERVERNAME + " 003 " + client + " " \
		+ ":This server was created " + datetime + "\r\n") // 003 - Server::sendWelcome

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:04:29 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/12 13:33:44 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "Server.hpp"

# define RPL_WELCOME(client) ( ":" + SERVERNAME + " 001 " + client + " " \
		+ ":Welcome to the " + SERVERNAME + " Network, " + client + "!" + client + "@<host>]\r\n") // 001 - Server::sendWelcome

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 08:34:04 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/07 15:58:49 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include "colors.hpp"
#include <poll.h>
#include <csignal>
#include <cstdlib>
#include "Server.hpp"

enum ErrorNum {
	SUCCESS,
	ARG_NB,
	GETADDRINFO,
	SOCKET_OPTION,
	SOCKET_BIND,
	LISTEN,
};

class WrongArgs : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "Error: Wrong arg format!"; }
};

class WrongPort : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "Error: Port number should be comprised between 0 and 65536!"; }
};

class GetAddrInfo : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "Error: GetAddrInfo function!"; }
};

class SetSockOpt : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "ERROR: Cannot set any option for the server socket!"; }
};

class BindError : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "ERROR: Server failed to bind to IP/Port!"; }
};

class Listen : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "ERROR: Server can't listen!"; }
};

#endif

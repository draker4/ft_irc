/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 08:34:04 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/06 15:42:18 by bperriol         ###   ########lyon.fr   */
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

enum ErrorNum {
	SUCCESS,
	ARG_NB,
	GETADDRINFO,
	SOCKET_OPTION,
	SOCKET_BIND,
	LISTEN,
};

#define MAX_CLIENTS 10

#endif

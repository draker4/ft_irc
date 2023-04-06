/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboisson <bboisson@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 08:34:04 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/06 09:51:46 by bboisson         ###   ########lyon.fr   */
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
#include <signal.h>

enum ErrorNum {
	ARG_NB,
	SOCKET_CREATION,
};

#define MAX_CLIENTS 10

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 08:34:04 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/06 14:22:58 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include "colors.hpp"

enum ErrorNum : char {
	ARG_NB,
	SOCKET_CREATION,
};

#endif
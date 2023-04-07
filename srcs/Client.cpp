/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:55:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 15:48:30 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// destructor

Client::~Client()
{}

// constructors

Client::Client(void)
{}

Client::Client(int serverSocket) : AUser(serverSocket)
{}

Client::Client(const Client &src) : AUser(src)
{
	*this = src;
}

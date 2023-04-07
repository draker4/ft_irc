/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:55:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 14:17:13 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// destructor

Client::~Client()
{}

// constructors

Client::Client(void)
{}

Client::Client(const Client &src)
{
	*this = src;
}

// operator overloads

Client	Client::operator=(const Client &rhs)
{
	// copy here
	return *this;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:55:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 17:12:31 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Client::Client(void)
{
	// std::cout << GREEN << "Client Default Constructor called "
	// 	<< RESET << std::endl;
}

Client::Client(int serverSocket) : AUser(serverSocket)
{
	// std::cout << GREEN << "Client Constructor called "
	// 	<< RESET << std::endl;
}

Client::Client(const Client &src) : AUser(src)
{
	// std::cout << GREEN << "Client Copy Constructor called "
	// 	<< RESET << std::endl;
	*this = src;
}

/* -----------------------------  Destructors  ------------------------------ */

Client::~Client()
{
	// std::cout << RED << "Client Destructor called "
	// 	<< RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Client	&Client::operator=(const Client &rhs)
{
	// std::cout << GREEN << "Client Assignment Operator called "
	// 	<< RESET << std::endl;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

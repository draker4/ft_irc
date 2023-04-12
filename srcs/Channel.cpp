/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/12 15:38:09 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Channel::Channel(void)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Default Constructor called " << RESET << std::endl;
}

Channel::Channel(const Channel &src)
{
	*this = src;
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Copy Constructor called " << RESET << std::endl;
}

/* -----------------------------  Destructors  ------------------------------ */

Channel::~Channel(void)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Default Destructor called " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Channel	&Channel::operator=(const Channel &rhs)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Assignment operator called " << RESET << std::endl;
}

/* --------------------------------  Getter  -------------------------------- */

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 13:57:00 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ABot.hpp"

/* -----------------------------  Constructors  ----------------------------- */

ABot::ABot(void)
{
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Default Constructor called " << RESET << std::endl;
}

ABot::ABot(const ABot &src)
{
	*this = src;
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Copy Constructor called " << RESET << std::endl;
}

/* -----------------------------  Destructors  ------------------------------ */

ABot::~ABot(void)
{
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Default Destructor called " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

ABot	&ABot::operator=(const ABot &rhs)
{
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Assignment operator called " << RESET << std::endl;
	(void)rhs;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

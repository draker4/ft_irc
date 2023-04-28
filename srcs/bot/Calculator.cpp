/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 13:57:00 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Calculator.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Calculator::Calculator(void)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Default Constructor called " << RESET << std::endl;
}

Calculator::Calculator(const Calculator &src)
{
	*this = src;
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Copy Constructor called " << RESET << std::endl;
}

/* -----------------------------  Destructors  ------------------------------ */

Calculator::~Calculator(void)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Default Destructor called " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Calculator	&Calculator::operator=(const Calculator &rhs)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Assignment operator called " << RESET << std::endl;
	(void)rhs;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

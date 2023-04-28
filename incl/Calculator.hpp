/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:32 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 12:40:31 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CALCULATOR_HPP
# define CALCULATOR_HPP

# include <iostream>
# include "colors.hpp"

# define DEBUG_CALCULATOR false

class	Calculator
{
	public:
		// Types
		
		//Constructors
		Calculator(void);

		//Destructor
		~Calculator(void);

		//Operators
		
		//Getter

		//Setter

		//Public functions

		//Exceptions
		class CalculatorException : public std::exception
		{
			private:
				const char	*_msg;
			public:
				CalculatorException( const char *msg ) : _msg( msg ) {}
				virtual const char *what() const throw() {
					return ( _msg );
				};
		};

	private:

		//Private Constructor
		Calculator(const Calculator &src);

		//Private Operator
		Calculator	&operator=(const Calculator &rhs);

		// Private functions
};

#endif

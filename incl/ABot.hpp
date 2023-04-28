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

#ifndef ABOT_HPP
# define ABOT_HPP

# include <iostream>
# include "colors.hpp"

# define DEBUG_ABOT false

class	ABot
{
	public:
		// Types
		
		//Constructors
		ABot(void);

		//Destructor
		~ABot(void);

		//Operators
		
		//Getter

		//Setter

		//Public functions

		//Exceptions
		class ABotException : public std::exception
		{
			private:
				const char	*_msg;
			public:
				ABotException( const char *msg ) : _msg( msg ) {}
				virtual const char *what() const throw() {
					return ( _msg );
				};
		};

	private:

		//Private Constructor
		ABot(const ABot &src);

		//Private Operator
		ABot	&operator=(const ABot &rhs);

		// Private functions
};

#endif

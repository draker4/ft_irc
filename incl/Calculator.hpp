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
# include <csignal>
# include <unistd.h>
# include <stdlib.h>
# include <cstring>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include "colors.hpp"

# define DEBUG_CALCULATOR true
# define BOTNAME std::string("Calculator")
# define BUFFER 4096

class	Calculator
{
	public:
		// Types
		
		//Constructors
		Calculator(std::string port, std::string password);

		//Destructor
		~Calculator(void);

		//Operators
		
		//Getter

		//Setter

		//Public functions
		void		launch(void);
		void		sendServer(const std::string &msg, const int &clientSocket) const;

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
		int			_sockfd;
		double		_port;
		std::string	_password;

		//Private Constructor
		Calculator(void);
		Calculator(const Calculator &src);

		//Private Operator
		Calculator	&operator=(const Calculator &rhs);

		// Private functions
		void	_connexion(void);
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:21:27 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/27 16:56:00 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <algorithm>
# include <iostream>
# include <vector>
# include <iostream>
# include "colors.hpp"

# define DEBUG_MESSAGE true

class	Message
{
	public:
		//Types
		typedef	std::vector<std::string>			vecString;
		typedef	std::vector<std::string>::iterator	itVecString;
		typedef	std::string::iterator				itString;

		//Constructors
		Message(std::string message);
		Message(const Message &src);

		//Destructor
		~Message(void);

		// Assignment Operator
		Message	&operator=(const Message &rhs);

		// Getter
		vecString	getTags(void) const;
		std::string	getSource(void) const;
		std::string	getCommand(void) const;
		vecString	getParameters(void) const;
		
		// Setter

		// Publice member functions

		// Exceptions
		class ErrorMsgFormat : public std::exception
		{
			public:
				virtual const char	*what() const throw() { return "ERROR: Message not correctly formated!"; }
		};
	
	private:
		vecString	_tags;
		std::string	_source;
		std::string	_command;
		vecString	_parameters;

		//Constructors
		Message(void);

		//Private functions
		void		_printReceive(void);
		vecString	_split(std::string message, std::string character);
};

#endif

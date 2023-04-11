/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:21:27 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/11 12:51:34 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

// # include "ircserv.hpp"

# include <algorithm>
# include <vector>
# include <iostream>

class	Message
{
	public:
		typedef	std::vector<std::string>			vecString;
		typedef	std::vector<std::string>::iterator	itVecString;
		typedef	std::string::iterator				itString;

		Message(std::string message);
		~Message(void);
		
		Message(const Message &src);
		
		Message	&operator=(const Message &rhs);

		vecString	getTags(void) const;
		std::string	getSource(void) const;
		std::string	getCommand(void) const;
		vecString	getParameters(void) const;
		
		class ErrorMsgFormat : public std::exception
		{
			public:
				virtual const char	*what() const throw() { return "ERROR: Message not correctly formated!"; }
		};
	
	private:
		Message(void);

		vecString	_tags;
		std::string	_source;
		std::string	_command;
		vecString	_parameters;
		
		vecString	_split(std::string message, std::string character);
};

#endif

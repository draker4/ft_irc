/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:21:27 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/10 19:56:29 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <iostream>
# include <vector>

class	Message
{
	private:
		Message(void);

		std::vector<std::string>	_tags;
		std::string					_source;
		std::string					_command;
		std::vector<std::string>	_parameters;
		
		std::vector<std::string>	split(std::string message, std::string character);

	public:
	Message(std::string message);
	~Message(void);
	
	Message(const Message &src);
	
	Message	&operator=(const Message &rhs);

	std::string getTags(void) const;
	std::string getSource(void) const;
	std::string getCommand(void) const;
	std::string getParameters(void) const;
};

#endif

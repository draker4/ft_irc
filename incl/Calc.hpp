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

# include "ABot.hpp"
# include <sstream>

# define DEBUG_CALCULATOR true

class	Calc : public ABot
{

	public:
		// Types
		
		//Constructors
		Calc(std::string servername, std::string port, std::string password);

		//Destructor
		~Calc(void);

		//Operators
		
		//Getter

		//Setter

		//Public functions
		virtual void	bot_purpose(void) const;
		void			joinCalcChannel(void) const;

	private:

		//Private Constructor
		Calc(void);
		Calc(const Calc &src);

		//Private Operator
		Calc	&operator=(const Calc &rhs);

		// Private functions
		std::string	_calc(std::string to_parse) const;
};

#endif

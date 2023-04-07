/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IUser.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:35:01 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 14:24:20 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IUSER_HPP
# define IUSER_HPP

# include "ircser.hpp"

class	IUser
{
	protected:
	
		std::string	_nickname;
		std::string	_real_name;
		std::string	_username;
	
	public:
	
		virtual ~IUser() {};
		
};

#endif

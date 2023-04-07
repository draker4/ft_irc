/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IUser.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:35:01 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 15:37:11 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IUSER_HPP
# define IUSER_HPP

# include "ircserv.hpp"

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

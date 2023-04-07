/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:33:16 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 13:56:57 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "IUser.hpp"

class Client : public AUser
{
	private:
	
	public:

		~Client(void);
		Client(void);
		Client(const Client &src);
		
		Client	&opertor=(const Client &rhs);
}

#endif

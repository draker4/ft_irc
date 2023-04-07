/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:33:16 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 17:12:12 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AUser.hpp"

class Client : public AUser
{
	private:
		// Constructors
		Client(void);
		// Private functions
		
	public:

		// Constructors
		Client(int _serverSocket);
		Client(const Client &src);
		
		// Destructors
		virtual ~Client(void);
		
		// Assignment Operator
		Client	&operator=(const Client &rhs);
		
		// Getter
		
		// Setter
		
		// Public member functions
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:33:16 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 17:44:43 by bperriol         ###   ########lyon.fr   */
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
		
		// Getter
		
		// Setter
		
		// Public member functions
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:33:16 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 15:20:48 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AUser.hpp"

class Client : public AUser
{
	private:
	
		Client(void);
		
	public:

		virtual ~Client(void);
		Client(int _serverSocket);
		Client(const Client &src);
};

#endif

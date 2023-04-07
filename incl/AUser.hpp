/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AUser.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:35:01 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 17:37:36 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUSER_HPP
# define AUSER_HPP

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "colors.hpp"


class Accept : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "ERROR: Server can't accept connection!"; }
};

class	AUser
{
	private:
		// Private functions
		void	*get_addr(sockaddr *saddr);

	protected:
		int					_serverSocket;
		int					_clientSocket;
		char				_inet[INET6_ADDRSTRLEN];
		std::string			_nickname;
		std::string			_real_name;
		std::string			_username;
		sockaddr_storage	_client_addr;
		socklen_t			_client_addr_size;

		// Constructors
		AUser(void);
		AUser(const AUser &src);

	public:
		// Constructors
		AUser(int serverSocket);
		
		// Destructors
		virtual ~AUser(void);
		
		// Assignment Operator
		virtual AUser	&operator=(const AUser &rhs);
		
		// Getter
		char	*getInet(void);
		int		getClientSocket(void) const;
		
		// Setter
		void	setClientSocket(void);
		
		// Public member functions
		
};

#endif

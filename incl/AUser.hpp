/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AUser.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:35:01 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 15:58:59 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUSER_HPP
# define AUSER_HPP

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>


class Accept : public std::exception
{
	public:
		virtual const char	*what() const throw() { return "ERROR: Server can't accept connection!"; }
};

class	AUser
{
	private:

		void				*get_addr(sockaddr *saddr);

	protected:
	
		AUser(void);
		AUser(const AUser &src);

		int					_serverSocket;
		int					_clientSocket;
		char				_inet[INET6_ADDRSTRLEN];
		std::string			_nickname;
		std::string			_real_name;
		std::string			_username;
		sockaddr_storage	_client_addr;
		socklen_t			_client_addr_size;
	
	public:
	
		virtual ~AUser(void);
		AUser(int serverSocket);
		
		AUser				&operator=(const AUser &rhs);
		
		void				setClientSocket(void);
		
		int					getClientSocket(void) const;
		char				*getInet(void);
};

#endif

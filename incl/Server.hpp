/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 12:24:33 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ircServ.hpp"

class Server
{
	private:

		Server(void);

		int			_serverSocket;
		int			_port;
		int			_reuse;
		std::string	_password;
		
	public:

		~Server(void);
		Server(std::string port, std::string password);
		Server(const Server &src);
		
		Server	&operator=(const Server &rhs);

		int	getServerSocket(void) const;
};

#endif

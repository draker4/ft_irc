/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 16:33:23 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ircserv.hpp"
#include "Client.hpp"

#define MAX_CLIENTS 10

class Server
{
	private:

		Server(void);
		Server(const Server &src);

		Server				&operator=(const Server &rhs);
		
		int					_serverSocket;
		int					_port;
		int					_reuse;
		std::string			_password;
		std::vector<pollfd>	_fds;
		std::vector<Client>	_clients;

	public:

		~Server(void);
		Server(std::string port, std::string password);
		

		int					getServerSocket(void) const;
		
		void				init(void);
		void				launch(void);
};

#endif

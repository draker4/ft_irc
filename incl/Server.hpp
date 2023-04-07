/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 12:57:45 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ircServ.hpp"

#define MAX_CLIENTS 10

class Server
{
	private:

		Server(void);
		
		void				*get_addr(sockaddr *saddr);

		int					_serverSocket;
		int					_clientSocket;
		int					_port;
		int					_reuse;
		int					_nbClients;
		std::string			_password;
		pollfd				_fds[MAX_CLIENTS + 1]; // plus 1 for the listening socket
		sockaddr_storage	_client_addr;
		char				inet[INET6_ADDRSTRLEN];
		
	public:

		~Server(void);
		Server(std::string port, std::string password);
		Server(const Server &src);
		
		Server				&operator=(const Server &rhs);

		int					getServerSocket(void) const;
		
		void				init(void);
		void				launch(void);
};

#endif

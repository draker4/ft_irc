/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 17:15:06 by baptiste         ###   ########lyon.fr   */
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
		int					_serverSocket;
		int					_port;
		int					_reuse;
		std::string			_password;
		std::vector<pollfd>	_fds;
		std::vector<Client>	_clients;
		
		// Constructors
		Server(void);
		Server(const Server &src);
		
		// Assignment Operator
		Server	&operator=(const Server &rhs);

	public:
		// Constructors
		Server(std::string port, std::string password);
		
		// Destructors
		~Server(void);

		// Getter
		int		getServerSocket(void) const;
		
		// Setter

		// Public member functions
		void	init(void);
		void	launch(void);
};

#endif

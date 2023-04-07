/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 19:05:37 by bperriol         ###   ########lyon.fr   */
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

		// Private member functions
		void	addUser(std::vector<pollfd> &new_fds);
		void	receive_data(std::vector<pollfd>::iterator &it);

	public:
		// Constructors
		Server(std::string port, std::string password);
		
		// Destructors
		~Server(void);

		// Getter
		int		getServerSocket(void) const;
		
		// Setter

		// Public member functions
		void	launch(void);
};

#endif

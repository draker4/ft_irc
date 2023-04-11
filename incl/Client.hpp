/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 13:35:01 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/10 15:16:39 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <cstdlib>
# include <netinet/in.h>
# include <arpa/inet.h>
# include "colors.hpp"

# define USERLEN 10

class Client
{
	private:
		// Private functions
		void *get_addr(sockaddr *saddr);

	protected:
		int 				_serverSocket;
		int					_clientSocket;
		char 				_inet[INET6_ADDRSTRLEN];
		std::string			_nickname;
		std::string			_real_name;
		std::string 		_username;
		std::string			_buffer;
		std::string			_buffer_send;
		sockaddr_storage	_client_addr;
		socklen_t 			_client_addr_size;
		bool 				_registered;
		bool				_password_ok;
		bool				_deconnect;

		// Constructors
		Client(void);
		Client(const Client &src);

	public:
		// Constructors
		Client(int serverSocket);

		// Destructors
		~Client(void);

		// Assignment Operator
		Client &operator=(const Client &rhs);

		// Getter
		char 		*getInet(void);
		int 		getClientSocket(void) const;
		bool 		getRegistered(void) const;
		bool		getDeconnect(void) const;
		bool		getPassword(void) const;
		std::string	getBuffer(void) const;
		std::string	getBufferSend(void) const;
		std::string	getNickname(void) const;

		// Setter
		void 		setNickname(std::string nickname);
		int 		setClientSocket(void);
		void		setDeconnect(bool boolean);
		void		setPassword(bool boolean);
		void		setUsername(std::string username);
		void		setRealName(std::string realname);
		void		setRegistered(bool boolean);

		// Public member functions
		void		addBuffer(std::string to_add);
		void		addBufferSend(std::string to_add);
		void		clearBuffer(void);
		void		clearBufferSend(void);
};

#endif

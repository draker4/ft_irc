/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:32 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 12:40:31 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABOT_HPP
# define ABOT_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string.h>
# include <cstdlib>
# include <unistd.h>
# include "colors.hpp"
# include <poll.h>
# include "Message.hpp"
# include <arpa/inet.h>

# define DEBUG_ABOT false
# define BUFFER 4096
# define BOTNAME std::string("Calculator")

class	ABot
{
	public:
		// Types
		
		//Constructors
		ABot(std::string servername, std::string port, std::string password);

		//Destructor
		virtual ~ABot(void);

		//Operators
		
		//Getter

		//Setter

		//Public functions
		void	connectToServer(void);
		void	createUser(void);
		void	sendServer(const std::string &msg) const;
		void	launch(void);
		
		// abstract function
		virtual void	bot_purpose(void) const = 0;

		//Exceptions
		class ABotException : public std::exception
		{
			private:
				const char	*_msg;
			public:
				ABotException( const char *msg ) : _msg( msg ) {}
				virtual const char *what() const throw() {
					return ( _msg );
				};
		};

	protected:

		const	std::string	_servername;
		const	std::string	_port;
		const	std::string	_password;
		std::string			_recvBuffer;
		char				_inet[INET6_ADDRSTRLEN];
		int					_clientSocket;
		int					_reuse;
		bool				_registered;

		// protected constructor
		ABot(void);
		ABot(const ABot &src);

		//protected Operator
		ABot	&operator=(const ABot &rhs);

		// Private functions
		void	*_get_in_addr(struct sockaddr *sa);
		void	_receiveData(void);
		void	_addRecvBuffer(std::string buffer);
};

#endif

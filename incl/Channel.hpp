/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:32 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/13 18:11:14 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "colors.hpp"
# include <map>

# define DEBUG_CHANNEL false

class Client;

class	Channel
{
	public:
		// Types
		typedef std::map<Client *, std::string>				mapClients;
		typedef std::map<Client *, std::string>::iterator	itMapClients;
		
		//Constructors
		Channel(void);
		Channel(const Channel &src);

		//Destructor
		~Channel(void);

		//Assignement operator
		Channel	&operator=(const Channel &rhs);
		
		// getter
		mapClients	getClients(void) const;

		// setter

		// Public member functions

		//Exceptions
		class ChannelException : public std::exception
		{
			private:
				const char	*_msg;
			public:
				ChannelException( const char *msg ) : _msg( msg ) {}
				virtual const char *what() const throw() {
					return ( _msg );
				};
		};

	private:
		std::string			_key;
		mapClients			_clients;
		// const unsigned int	_client_limit;

		// Private functions
};

#endif

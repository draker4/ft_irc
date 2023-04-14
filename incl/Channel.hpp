/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:32 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 16:28:36 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>
# include <vector>
# include "colors.hpp"

# define DEBUG_CHANNEL false

class Client;

typedef struct	s_connect
{
	Client		*client;
	char		oper;
	char		prefix;
}				t_connect;

class	Channel
{
	public:
		// Types
		typedef std::map<std::string, t_connect>					mapClients;
		typedef std::map<std::string, t_connect>::const_iterator	itMapClients;
		typedef std::vector<std::string>							vecNickName;
		typedef std::vector<std::string>::const_iterator			itVecNickName;
		typedef std::string::const_iterator							itString;
		
		//Constructors
		Channel(std::string name, Client *client);

		//Destructor
		~Channel(void);
		
		// getter
		char			getSymbol(void) const;
		char			getPrefix(Client *client) const;
		std::string		getName(void) const;
		std::string		getKey(void) const;
		mapClients		getClients(void) const;
		bool			getModeStatus(char c) const;

		// setter

		// Public member functions
		void	addClient(Client *client);
		bool	isBanned(std::string nickname) const;
		bool	isFull(void) const;
		bool	isInvited(std::string nickname) const;

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
		char			_symbol;
		std::string 	_name;
		std::string		_key;
		mapClients		_clients;
		vecNickName		_banned;
		vecNickName		_invited;
		std::string		_mode;
		std::string		_topic;
		unsigned int	_clientLimit;
		time_t			_t_create;
		std::string		_t_create_str;

		//constructor
		Channel(void);
		Channel(const Channel &src);

		//Assignement operator
		Channel	&operator=(const Channel &rhs);

		// Private functions
};

#endif

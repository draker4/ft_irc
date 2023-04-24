/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:32 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/24 12:27:12 by bperriol         ###   ########lyon.fr   */
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
	std::string	userMode;
	std::string	prefix;
	std::string	joinTime;
}				t_connect;

class	Channel
{
	public:
		// Types
		typedef std::map<std::string, t_connect>				mapClients;
		typedef std::map<std::string, t_connect>::iterator		itMapClients;
		typedef std::map<std::string, std::string>				mapMode;
		typedef std::map<std::string, std::string>::iterator	itMapMode;
		typedef std::string::const_iterator						itConstString;
		typedef std::string::iterator							itString;
		
		//Constructors
		Channel(std::string name, Client *client);

		//Destructor
		~Channel(void);
		
		// getter
		char			getSymbol(void) const;
		std::string		getPrefix(std::string nickName);
		std::string		getName(void) const;
		std::string		getKey(void) const;
		std::string		getTopic(void) const;
		mapClients		getClients(void) const;
		bool			getModeStatus(char c) const;
		std::string		getMode(void) const;
		std::string		getTimeCreated(void);
		std::string		getTimeTopic(void);
		int				getOperGrade(std::string nickName);

		// setter
		void			setTopic(std::string topic);
		void			setKey(std::string key);
		void			setClientLimit(std::string limit);

		// Public member functions
		void	addClient(Client *client);
		bool	isBanned(std::string nickname) const;
		bool	isFull(void) const;
		bool	isInvited(std::string nickname) const;
		bool	isClientInChannel(std::string nickname) const;
		void	addBanned(std::string nickname);
		void	addInvited(std::string nickname);
		void	removeBanned(std::string nickname);
		void	removeInvited(std::string nickname);
		void	addMode(char c);
		void	removeMode(char c);

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
		mapMode			_banned;
		mapMode			_invited;
		std::string		_mode;
		std::string		_topic;
		std::string		_topicTime;
		unsigned int	_clientLimit;
		std::string		_timeCreated;
		std::string		_timeLastTopic;

		//constructor
		Channel(void);
		Channel(const Channel &src);

		//Assignement operator
		Channel	&operator=(const Channel &rhs);

		// Private functions
};

#endif

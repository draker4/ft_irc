/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:32 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/25 10:57:27 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>
# include <vector>
# include "colors.hpp"
# include <iterator>
# include "replies.hpp"

# define DEBUG_CHANNEL false

class Client;

typedef struct	s_connect
{
	Client		*client;
	std::string	userMode;
	std::string	prefix;
	std::string	joinTime;
}				t_connect;

typedef struct	s_ban
{
	std::string	banBy;
	std::string	time;
}				t_ban;

class	Channel
{
	public:
		// Types
		typedef std::map<std::string, t_connect>					mapClients;
		typedef std::map<std::string, t_connect>::iterator			itMapClients;
		typedef std::map<std::string, std::string>					mapInv;
		typedef std::map<std::string, std::string>::iterator		itMapInv;
		typedef std::map<std::string, std::string>::const_iterator	constItMapInv;
		typedef std::map<std::string, t_ban>						mapBan;
		typedef std::map<std::string, t_ban>::iterator				itMapBan;
		typedef std::map<std::string, t_ban>::const_iterator		constItMapBan;
		typedef std::string::const_iterator							itConstString;
		typedef std::string::iterator								itString;
		
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
		bool			getUserModeStatus(std::string nickName, char c);
		std::string		getMode(void) const;
		std::string		getTimeCreated(void);
		std::string		getTimeTopic(void);
		int				getOperGrade(std::string nickName);
		std::string		getClientTopic(void) const;
		unsigned int	getClientLimit(void) const;
		mapBan			getBanList(void) const;

		// setter
		void			setTopic(std::string nickname, std::string topic);
		void			setKey(std::string key);
		void			setClientLimit(std::string limit);

		// Public member functions
		void	addClient(Client *client);
		void	removeClient(Client *client);
		bool	isBanned(std::string ban) const;
		bool	isFull(void) const;
		bool	isInvited(std::string nickname);
		bool	isClientInChannel(std::string nickname) const;
		bool	addBanned(std::string ban, std::string banBy);
		void	addInvited(std::string nickname);
		bool	removeBanned(std::string nickname);
		void	removeInvited(std::string nickname);
		void	addMode(char c);
		void	removeMode(char c);
		void	updateClient(std::string oldNickname, std::string nickname);
		void	addUserMode(std::string nickname, char c);
		void	removeUserMode(std::string nickname, char c);

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
		mapBan			_banned;
		mapInv			_invited;
		std::string		_mode;
		std::string		_topic;
		std::string		_client_topic;
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

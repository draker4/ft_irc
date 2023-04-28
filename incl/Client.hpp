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
# include <cstdio>
# include <netinet/in.h>
# include <arpa/inet.h>
# include "colors.hpp"
# include "Channel.hpp"
# include <vector>

// RPL_ISUPPORT
# define USERLEN 10

# define DEBUG_CLIENT false

class Channel;
class Client
{
	public:
		// Types
		typedef std::vector<Channel *>					vecChannel;
		typedef std::vector<Channel *>::iterator		itVecChannel;
		typedef std::vector<Channel *>::const_iterator	constItVecChannel;
		typedef std::string::const_iterator				itString;

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
		bool		getPasswordStatus(void) const;
		std::string	getBuffer(void) const;
		std::string	getNickName(void) const;
		std::string	getOldNickName(void) const;
		std::string	getUserName(void) const;
		std::string	getRealName(void) const;
		std::string	getMode(void) const;
		std::string	getReasonLeaving() const;
		bool		getModeStatus(char c) const;
		vecChannel	getChannels(void) const;

		// Setter
		int 		setClientSocket(void);
		void		setDeconnect(bool boolean);
		void		setPasswordStatus(bool boolean);
		void 		setNickName(std::string nickName);
		void		setUserName(std::string userName);
		void		setRealName(std::string realName);
		void 		setOldNickName(std::string oldNickName);
		void		setRegistered(bool boolean);
		void		setReasonLeaving(std::string reason);

		// Public member functions
		void		addBuffer(std::string to_add);
		void		clearBuffer(void);
		void		addMode(char c);
		void		removeMode(char c);
		void		addChannel(Channel *channel);
		void		removeChannel(Channel *channel);
		bool		shareChannel(vecChannel channels) const;
	
	private:
		int 					_serverSocket;
		int						_clientSocket;
		char 					_inet[INET6_ADDRSTRLEN];
		std::string				_nickName;
		std::string				_oldNickName;
		std::string				_realName;
		std::string 			_userName;
		std::string				_buffer;
		std::string				_mode;
		std::string				_reason_leaving;
		sockaddr_storage		_client_addr;
		socklen_t 				_client_addr_size;
		bool 					_registered;
		bool					_passwordSet;	
		bool					_deconnect;
		vecChannel				_channels;

		// Constructors
		Client(void);
		Client(const Client &src);

		// Private functions
		void	*_get_addr(sockaddr *saddr);
};

#endif

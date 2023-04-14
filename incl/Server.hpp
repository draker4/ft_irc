/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 13:09:04 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <fstream>
# include <unistd.h>
# include <stdlib.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <string>
# include <cstring>
# include <cstdio>
# include <poll.h>
# include <csignal>
# include <cstdlib>
# include <ctime>
# include <vector>
# include <map>
# include "colors.hpp"
# include "errors.hpp"
# include "replies.hpp"
# include "Message.hpp"
# include "Client.hpp"
# include "command.hpp"
# include <sstream>
# include <sys/time.h>
# include <cstdlib>

# define DEBUG_SERVER true
# define SERVERNAME std::string("localhost")
# define VERSION std::string("1.0")
# define MAX_CLIENTS 1000

enum ErrorNum {
	SUCCESS,
	FAILURE,
	ARG_NB,
};

typedef struct	s_opeConf
{
	std::string 	name;
	std::string		host;
	std::string 	password;
}				t_opeConfig;

class Server
{
	public:
		// Types
		typedef std::map<int, Client *>							mapClient;
		typedef std::map<int, Client *>::iterator				itMapClient;
		typedef std::map<int, Client *>::const_iterator 		constItMapClient;
		typedef std::vector<pollfd> 							vecPollfd;
		typedef std::vector<pollfd>::iterator					itVecPollfd;
		typedef void (*CmdFunction)(Client *, const Message &, Server*);
		typedef std::map<std::string, CmdFunction> 				mapCommand;
		typedef std::map<std::string, CmdFunction>::iterator	itMapCommand;
		typedef std::vector<t_opeConfig>						vecOpeConfig;
		typedef std::vector<t_opeConfig>::iterator				itVecOpeConfig;
		typedef std::vector<Channel *>							vecChannel;
		typedef std::vector<Channel *>::iterator				itVecChannel;

		// Constructors
		Server(std::string port, std::string password);

		// Destructors
		~Server(void);

		// Getter
		int				getServerSocket(void) const;
		std::string		getPassword(void) const;
		mapClient		getClients(void) const;
		Client			*getClient(std::string nickname) const;
		vecOpeConfig	getOpeConf(void) const;

		// Setter

		// Public member functions
		void		launch(void);
		void		sendClient(const std::string &msg, const int &clientSocket) const;
		void		sendWelcome(Client *client) const;
		void		deleteClient(Client *client);
	
		// Exceptions
		class ServerException : public std::exception
		{
			private:
				const char	*_msg;
			public:
				ServerException( const char *msg ) : _msg( msg ) {}
				virtual const char *what() const throw() {
					return ( _msg );
				};
		};

	private:
		int				_serverSocket;
		int				_port;
		int 			_reuse;
		std::string		_password;
		vecPollfd		_fds;
		mapClient		_clients;
		mapCommand		_commands;
		time_t			_t_create;
		std::string		_t_create_str;
		vecOpeConfig	_opeConf;

		// Constructors
		Server(void);
		Server(const Server &src);

		// Assignment Operator
		Server &operator=(const Server &rhs);

		// Private functions
		void	_addUser(vecPollfd &new_fds);
		void	_receiveData(itVecPollfd &it);
		void	_handleCommand(std::string msg, int clientSocket);
		void	_initCommands(void);
		void	_initOperatorConfig(void);
		void	_deleteClient(itVecPollfd it);
		Client	*_getClient(int clientSocket);
};

#endif


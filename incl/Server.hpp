/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 13:55:39 by bperriol         ###   ########lyon.fr   */
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
# define SERVERINFO std::string("This an IRC server built by bperriol and bboisson")

# define ACCEPT 20
# define BOT std::string("B")
# define CHANLIMIT 10
# define CHANMODE std::string("bhiklmnostv")
# define CHANNELLEN 50
# define CHANTYPES std::string("&#")
# define PREFIX std::string("(ohv)@%+")
# define USERMODE std::string("iorw")
# define MAX_CLIENTS 1000

// RPL_ISUPPORT
# define BOT_TOKEN std::string("BOT=" + BOT) // Indicates the character to be used as a user mode to let clients mark themselves as bots by setting it
# define CHANLIMIT_TOKEN std::string("CHANLIMIT=" + CHANTYPES + ":10") // Indicates the maximum number of channels a client may join
# define CHANMODES_TOKEN std::string("CHANMODES=" + CHANMODE) // Indicates the channel modes available on this server
# define CHANNELLEN_TOKEN std::string("CHANNELLEN=50") // Indicates the maximum length of a channel name
# define CHANTYPES_TOKEN std::string("CHANTYPES=" + CHANTYPES) // Indicates the types of channels supported on this server
# define KEYLEN_TOKEN std::string("KEYLEN=10") // Indicates the maximum length of a channel key.
# define KICKLEN_TOKEN std::string("KICKLEN=") // Indicates the maximum length of a kick message.
# define MAXLIST_TOKEN std::string("MAXLIST=") // Indicates the maximum number of entries in a channel mode list.
# define MAXNICKLEN_TOKEN std::string("MAXNICKLEN=9") // Indicates the maximum length of a nickname.
# define MAXTARGETS_TOKEN std::string("MAXTARGETS=") // Indicates the maximum number of targets allowed in a command requiring multiple targets.
# define MODES_TOKEN std::string("MODES=") // Indicates the maximum number of modes with parameter allowed to be set on a channel in a single MODE command.
# define TARGMAX_TOKEN std::string("TARGMAX=WHO:,WHOIS:1,NAMES:,PRIVMSG:,NOTICE:,JOIN:,PART:") // Indicates the maximum number of targets allowed in a command requiring multiple targets.
# define TOPICLEN_TOKEN std::string("TOPICLEN=") // Indicates the maximum length of a topic.
# define PREFIX_TOKEN std::string("PREFIX=" + PREFIX) // Indicates the channel membership prefixes available on this server and their order in terms of channel privileges they represent, from highest to lowest.
# define USERLEN_TOKEN std::string("USERLEN=""10") // Indicates the maximum length of an username in octets. If "number" is not specified, there is no limit.

# define TOKEN1 std::string(BOT_TOKEN + " " + CHANLIMIT_TOKEN + " " + CHANMODES_TOKEN \
		+ " " + CHANNELLEN_TOKEN + " " + CHANTYPES_TOKEN + " " + KEYLEN_TOKEN + " " \
		+ KICKLEN_TOKEN + " " + MAXLIST_TOKEN + " " + MAXNICKLEN_TOKEN)
# define TOKEN2 std::string(MAXTARGETS_TOKEN + " " + MODES_TOKEN + " " + TARGMAX_TOKEN \
		+ " " + TOPICLEN_TOKEN + " " + PREFIX_TOKEN + " " + USERLEN_TOKEN )

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
		typedef std::vector<Channel *>::const_iterator			constItVecChannel;
		typedef	std::vector<Client *>							vecClient;
		typedef	std::vector<Client *>::iterator					itVecClient;

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
		Channel			*getChannel(std::string name);
		vecClient		getClientsHost(std::string username_host) const;
		vecChannel		getChannels(void) const;

		// Setter

		// Public member functions
		void		launch(void);
		void		sendClient(const std::string &msg, const int &clientSocket) const;
		void		sendWelcome(Client *client);
		void		deleteClient(int clientSocket);
		void		addChannel(Channel *channel);
		void		removeChannel(Channel *channel);
		bool		channelExist(std::string name) const;
		bool		isClientInServer(std::string nickname) const;
	
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
		vecChannel		_channels;

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
		void	_sendQUIT(Client *client);
		Client	*_getClient(int clientSocket);
};

// non member functions
std::string	toUpper(std::string str);

#endif


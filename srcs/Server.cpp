/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:13 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/11 15:19:02 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool serverOpen;

/* -----------------------------  Constructors  ----------------------------- */

Server::Server(void)
{
	// std::cout << GREEN << "Server Default Constructor called "
	// 	<< RESET << std::endl;
}

Server::Server(std::string port, std::string password) : _port(strtod(port.c_str(), NULL)), _reuse(1), _password(password)
{
	// std::cout << GREEN << "Server Constructor called "
	// 	<< RESET << std::endl;
	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos || password.empty())
		throw Server::ServerException("Error: Wrong arg format!");
	double port_test = strtod(port.c_str(), NULL);
	if (port_test < 0 || port_test > 65536)
		throw Server::ServerException("Error: Port number should be comprised between 0 and 65536!");

	// set up the server adress struct
	addrinfo hints, *indexInfo, *serverInfo;
	int info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((info = getaddrinfo(NULL, port.c_str(), &hints, (addrinfo **)&serverInfo)) != 0)
	{
		std::cerr << RED << gai_strerror(info)
				  << RESET << std::endl;
		throw Server::ServerException("Error: GetAddrInfo function!");
	}

	// loop through all the results of getaddrinfo and bin the first possible
	for (indexInfo = serverInfo; indexInfo != NULL; indexInfo = indexInfo->ai_next)
	{
		// Create the socket
		if ((_serverSocket = socket(indexInfo->ai_family, indexInfo->ai_socktype, indexInfo->ai_protocol)) == -1)
		{
			std::cerr << RED << "ERROR: Cannot create socket!"
					  << RESET << std::endl;
			continue;
		}

		// set options for the socket
		if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuse, sizeof(int)) == -1)
		{
			close(_serverSocket);
			freeaddrinfo(serverInfo);
			throw Server::ServerException("ERROR: Cannot set any option for the server socket!");
		}

		// Bind the socket to the port
		if (bind(_serverSocket, indexInfo->ai_addr, indexInfo->ai_addrlen) == -1)
		{
			close(_serverSocket);
			std::cerr << RED << "ERROR: Cannot bind to IP/Port!"
					  << RESET << std::endl;
			continue;
		}

		break;
	}

	freeaddrinfo(serverInfo);

	if (!indexInfo)
	{
		close(_serverSocket);
		throw Server::ServerException("ERROR: Server failed to bind to IP/Port!");
	}

	// Listen for incoming connections
	if (listen(_serverSocket, SOMAXCONN) == -1)
	{
		close(_serverSocket);
		throw Server::ServerException("ERROR: Server can't listen!");
	}
	_initCommands();
}

Server::Server(const Server &src)
{
	// std::cout << GREEN << "Server Copy Constructor called "
	// 	<< RESET << std::endl;
	*this = src;
}

/* -----------------------------  Destructors  ------------------------------ */

Server::~Server(void)
{
	// std::cout << RED << "Server Destructor called "
	// 	<< RESET << std::endl;
	close(_serverSocket);

	// Close all client sockets
	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++)
	{
		close(it->fd);
	}

	// Delete all clients allocated by the server
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		delete it->second;
	}
	std::cout << YELLOW << "\nServer is shutting down... " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Server &Server::operator=(const Server &rhs)
{
	// std::cout << GREEN << "Server Assignment Operator called "
	// 	<< RESET << std::endl;
	_serverSocket = rhs._serverSocket;
	_port = rhs._port;
	_reuse = rhs._reuse;
	_password = rhs._password;
	_fds = rhs._fds;
	_clients = rhs._clients;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

int Server::getServerSocket(void) const
{
	return _serverSocket;
}

std::string	Server::getPassword(void) const
{
	return _password;
}

Server::mapClient Server::getClients(void) const
{
	return (_clients);
}

/* --------------------------------  Setter  -------------------------------- */

/* ----------------------  Private member functions  ------------------------ */

Client *Server::_getClient(int clientSocket)
{
	if (_clients[clientSocket])
		return _clients[clientSocket];
	throw ServerException("Error: Server can't find client!");
}

void Server::_addUser(vecPollfd &new_fds)
{
	Client *client = new Client(_serverSocket);

	// Accept the connection
	if (client->setClientSocket()) {
		delete client;
		throw Server::ServerException("ERROR: Server can't accept new client socket!");
	}

	// Add the client to the fds array
	if (_fds.size() == MAX_CLIENTS)
	{
		std::cerr << "ERROR: Too many clients!" << std::endl;
		close(client->getClientSocket());
		delete client;
	}
	else
	{
		pollfd client_fd;
		client_fd.fd = client->getClientSocket();
		client_fd.events = POLLIN;
		new_fds.push_back(client_fd);
		_clients[client_fd.fd] = client;
		std::cout << "New client connected, fd = " << client_fd.fd << std::endl;
		std::cout << GREEN << "Server got connection from " << client->getInet() << RESET << std::endl;
	}
}

void Server::_receiveData(itVecPollfd &it)
{
	char buf[4096];

	memset(buf, 0, sizeof(buf));
	int bytesReceived = recv(it->fd, buf, sizeof(buf), 0);
	if (bytesReceived == -1)
		std::cerr << "ERROR: Can't receive data from client!" << std::endl;
	else if (bytesReceived == 0)
	{
		std::cout << "Client disconnected, fd = " << it->fd << std::endl;
		close(it->fd);
		_clients.erase(it->fd);
		it--;
		_fds.erase(it + 1);
	}
	else
	{
		std::cout << YELLOW << "Server got :" << buf << "from " << it->fd << RESET << std::endl;
		_clients[it->fd]->addBuffer(std::string(buf, 0, sizeof(buf)));
		if (_clients[it->fd]->getBuffer().find_first_of("\r\n") != std::string::npos
			&& _clients[it->fd]->getBuffer()[_clients[it->fd]->getBuffer().length() - 2] == '\r')
		{
			_handleCommand(_clients[it->fd]->getBuffer(), it->fd);
			_clients[it->fd]->clearBuffer();
		}
	}
}

void Server::_handleCommand(std::string msg, int clientSocket)
{
	size_t end_line = -2;
	size_t begin_line = 0;

	while (msg[begin_line])
	{
		end_line = msg.find("\r\n", begin_line);
		try {
			Message 		message(msg.substr(begin_line, end_line));
			itMapCommand 	itCommand = _commands.find(message.getCommand());
			if (itCommand != _commands.end()) { // execute the command
				CmdFunction	execCommand = itCommand->second;
				execCommand(_getClient(clientSocket), message, this);
			} else { // the command is unknown, send something to the client
				std::cerr << RED << "command not found " << message.getCommand()
					<< RESET << std::endl;
				sendClient(ERR_UNKNOWNCOMMAND(std::string("0"), message.getCommand()), clientSocket);
				// try { 
					
				// } catch (Server::invalidFdException &e) {
				// 	printError(e.what(), 1, false);
				// }
			}
		}
		catch (const Message::ErrorMsgFormat &e) {
			std::cout << RED << e.what() << RESET << std::endl;
		}
		catch (const Server::ServerException &e) {
			std::cout << RED << e.what() << RESET << std::endl;
		}
		begin_line = end_line + 2;
	}
}

void Server::_initCommands(void)
{
	_commands["CAP"] = &cap;
	_commands["INVITE"] = &invite;
	_commands["JOIN"] = &join;
	_commands["KICK"] = &kick;
	_commands["KILL"] = &kill;
	_commands["LIST"] = &list;
	_commands["MODE"] = &mode;
	_commands["MOTD"] = &motd;
	_commands["NAMES"] = &names;
	_commands["NICK"] = &nick;
	_commands["NOTICE"] = &notice;
	_commands["OPER"] = &oper;
	_commands["PART"] = &part;
	_commands["PASS"] = &pass;
	_commands["PING"] = &ping;
	_commands["PRIVMSG"] = &privmsg;
	_commands["QUIT"] = &quit;
	_commands["TOPIC"] = &topic;
	_commands["USER"] = &user;
}

/* -----------------------  Public member functions  ------------------------ */

void Server::launch(void)
{

	pollfd server_fd;

	server_fd.fd = _serverSocket;
	server_fd.events = POLLIN;
	_fds.push_back(server_fd);

	// int	test = 0;

	while (serverOpen) // && test++ < 5)
	{
		std::vector<pollfd> new_fds;

		// Call poll()
		int ret = poll((pollfd *)&_fds[0], _fds.size(), -1);
		if (ret == -1 && serverOpen)
		{
			std::cerr << "ERROR: Poll failed!" << std::endl;
			break;
		}

		// Check for incoming connections
		itVecPollfd it = _fds.begin();

		// Check for incoming data on the client sockets
		for (it = _fds.begin(); it != _fds.end(); it++)
		{
			if (!it->revents)
				continue;
			if (it->revents & POLLIN)
			{
				if (it == _fds.begin())
				{
					try
					{
						_addUser(new_fds);
					}
					catch (const std::exception &e)
					{
						std::cerr << RED << e.what() << RESET << std::endl;
						continue;
					}
				}
				else
					_receiveData(it);
			}
		}
		_fds.insert(_fds.end(), new_fds.begin(), new_fds.end());
	}
}

void	Server::sendClient(const std::string &msg, const int &clientSocket) const
{
	size_t	bytes_sent = 0;

	while (bytes_sent < msg.length())
	{
		const int	len = send(clientSocket, &(msg.c_str())[bytes_sent], msg.length() - bytes_sent, 0);
		if (len < 0)
			throw ServerException("Error: Server can't send all bytes!");
		bytes_sent += len;
	}
}

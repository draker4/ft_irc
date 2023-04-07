/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:13 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 18:19:32 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool	serverOpen;

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
		throw WrongArgs();
	double	port_test = strtod(port.c_str(), NULL);
	if (port_test < 0 || port_test > 65536)
		throw WrongPort();
	
	//set up the server adress struct
	addrinfo	hints, *indexInfo, *serverInfo;
	int			info;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((info = getaddrinfo(NULL, port.c_str(), &hints, (addrinfo **)&serverInfo)) != 0) {
		std::cerr << RED << gai_strerror(info)
			<< RESET << std::endl;
		throw GetAddrInfo();
	}

	// loop through all the results of getaddrinfo and bin the first possible
	for (indexInfo = serverInfo; indexInfo != NULL; indexInfo = indexInfo->ai_next)
	{
		// Create the socket
		if ((_serverSocket = socket(indexInfo->ai_family, indexInfo->ai_socktype, indexInfo->ai_protocol)) == -1) {
			std::cerr << RED << "ERROR: Cannot create socket!"
				<< RESET << std::endl;
			continue ;
		}

		// set options for the socket
		if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuse, sizeof(int)) == -1)
		{
			close(_serverSocket);
			freeaddrinfo(serverInfo);
			throw SetSockOpt();
		}

		// Bind the socket to the port
		if (bind(_serverSocket, indexInfo->ai_addr, indexInfo->ai_addrlen) == -1) {
			close(_serverSocket);
			std::cerr << RED << "ERROR: Cannot bind to IP/Port!"
				<< RESET << std::endl;
			continue ;
		}

		break;
	}
	
	freeaddrinfo(serverInfo);

	if (!indexInfo)
	{
		close(_serverSocket);
		throw BindError();
	}
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
	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++) {
		close(it->fd);
	}
	
	std::cout << YELLOW << "\nServer is shutting down... " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Server	&Server::operator=(const Server &rhs)
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

int	Server::getServerSocket(void) const
{
	return _serverSocket;
}

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

void	Server::init(void)
{
	// Listen for incoming connections
	if (listen(_serverSocket, SOMAXCONN) == -1) {
		close(_serverSocket);
		throw Listen();
	}

	pollfd	server_fd;
	server_fd.fd = _serverSocket;
	server_fd.events = POLLIN;
	_fds.push_back(server_fd);
}

void	Server::launch(void)
{
	while (serverOpen) {
		
		std::vector<pollfd>	new_fds;
		
		// Call poll()
		int ret = poll((pollfd *)&_fds[0], _fds.size(), -1);
		if (ret == -1 && serverOpen) {
			std::cerr << "ERROR: Poll failed!" << std::endl;
			break;
		}
		
		// Check for incoming connections
		std::vector<pollfd>::iterator	it = _fds.begin();
		
		// Check for incoming data on the client sockets
		for (it = _fds.begin(); it != _fds.end(); it++)
		{
			if (it->revents & POLLIN)
			{
				if (it == _fds.begin()) {
					AUser	*client = new Client(_serverSocket);
					
					// Accept the connection
					
					try {
						client->setClientSocket();
					}
					catch (const std::exception &e) {
						std::cerr << RED << e.what() << RESET << std::endl;
						continue ;
					}

					// Add the client to the fds array
					if (_fds.size() == MAX_CLIENTS) {
						std::cerr << "ERROR: Too many clients!" << std::endl;
						close(client->getClientSocket());
					} else {
						pollfd	client_fd;
						client_fd.fd = client->getClientSocket();
						client_fd.events = POLLIN;
						new_fds.push_back(client_fd);
						std::cout << "New client connected, fd = " << client->getClientSocket() << std::endl;
					}
					std::cout << YELLOW << "Server got connection from " << client->getInet() << RESET << std::endl;
				}
				else
				{
					char buf[4096];
					memset(buf, 0, sizeof(buf));
					int bytesReceived = recv(it->fd, buf, sizeof(buf), 0);
					if (bytesReceived == -1) {
						std::cerr << "ERROR: Can't receive data from client!" << std::endl;
					} else if (bytesReceived == 0) {
						std::cout << "Client disconnected, fd = " << it->fd << std::endl;
						close(it->fd);
						it--;
						_fds.erase(it + 1);
					} else {
						std::cout << "Received: " << std::string(buf, 0, bytesReceived) 
							<< "from : " << it->fd << std::endl;
					}
				}
			}
		}
		_fds.insert(_fds.end(), new_fds.begin(), new_fds.end());
	}
	
}

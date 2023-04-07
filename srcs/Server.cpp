/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:13 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 12:56:20 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool	serverOpen;

// destructor

Server::~Server(void)
{
	close(_serverSocket);
}

// constructors

Server::Server(void)
{}

Server::Server(std::string port, std::string password) : _port(strtod(port.c_str(), NULL)), _reuse(1), _nbClients(0), _password(password)
{
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
	*this = src;
}

// operator overloads

Server	&Server::operator=(const Server &rhs)
{
	// copy here
	_serverSocket = rhs._serverSocket;
	_port = rhs._port;
	_reuse = rhs._reuse;
	_password = rhs._password;
	return *this;
}

// getter

int	Server::getServerSocket(void) const
{
	return _serverSocket;
}

// member functions


void	*Server::get_addr(sockaddr *saddr)
{
	if (saddr->sa_family == AF_INET)
		return &(((sockaddr_in *)saddr)->sin_addr);
	return &(((sockaddr_in6 *)saddr)->sin6_addr);
}

void	Server::init(void)
{
	// Listen for incoming connections
	if (listen(_serverSocket, SOMAXCONN) == -1) {
		close(_serverSocket);
		throw Listen();
	}

	// Set up the pollfd structures
	memset(_fds, 0, sizeof(_fds));
	_fds[0].fd = _serverSocket;
	_fds[0].events = POLLIN;
}

void	Server::launch(void)
{
	socklen_t			client_addr_size = sizeof(_client_addr);
	
	while (serverOpen) {
		
		// Call poll()
		int ret = poll(_fds, _nbClients + 1, -1);
		if (ret == -1 && serverOpen) {
			std::cerr << "ERROR: Poll failed!" << std::endl;
			break;
		}
		
		// Check for incoming connections
		if (_fds[0].revents & POLLIN) {
			
			// Accept the connection
			if ((_clientSocket = accept(_serverSocket, (sockaddr *)&_client_addr, &client_addr_size)) == -1) {
				std::cerr << "ERROR: Can't accept connection!" << std::endl;
				continue;
			}
			
			// get ipv4 or ipv6 address from client
			inet_ntop(_client_addr.ss_family, get_addr((sockaddr *)&_client_addr), inet, sizeof(inet));

			// Add the client to the fds array
			if (_nbClients == MAX_CLIENTS) {
				std::cerr << "ERROR: Too many clients!" << std::endl;
				close(_clientSocket);
			} else {
				_nbClients++;
				_fds[_nbClients].fd = _clientSocket;
				_fds[_nbClients].events = POLLIN;
				std::cout << "New client connected, fd = " << _clientSocket << std::endl;
			}
			std::cout << YELLOW << "Server got connection from " << inet << std::endl;
		}
		// Check for incoming data on the client sockets
		for (int i = 1; i <= _nbClients; i++) {
			if (_fds[i].revents & POLLIN) {
				char buf[4096];
				memset(buf, 0, sizeof(buf));
				int bytesReceived = recv(_fds[i].fd, buf, sizeof(buf), 0);
				if (bytesReceived == -1) {
					std::cerr << "ERROR: Can't receive data from client!" << std::endl;
				} else if (bytesReceived == 0) {
					std::cout << "Client disconnected, fd = " << _fds[i].fd << std::endl;
					close(_fds[i].fd);
					_nbClients--;
					_fds[i] = _fds[_nbClients + 1];
				} else {
					std::cout << "Received: " << std::string(buf, 0, bytesReceived) 
						<< "from : " << _fds[i].fd << std::endl;
				}
			}
		}
	}
	// Close all client sockets
	for (int i = 0; i <= _nbClients; i++) {
		close(_fds[i].fd);
	}
}

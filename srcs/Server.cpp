/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:13 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 12:25:21 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// destructor

Server::~Server(void)
{}

// constructors

Server::Server(void)
{}

Server::Server(std::string port, std::string password) : _port(strtod(port.c_str(), NULL)), _reuse(1), _password(password)
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

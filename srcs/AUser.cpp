/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AUser.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:44:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 15:56:31 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AUser.hpp"

// destructor

AUser::~AUser(void)
{}

// constructors

AUser::AUser(void)
{}

AUser::AUser(int serverSocket) : _serverSocket(serverSocket), _client_addr_size(sizeof(_client_addr))
{}

AUser::AUser(const AUser &src)
{
	*this = src;
}

// operator overloads

AUser	&AUser::operator=(const AUser &rhs)
{
	_nickname = rhs._nickname;
	_real_name = rhs._nickname;
	_username = rhs._username;
	_client_addr = rhs._client_addr;
	_client_addr_size = rhs._client_addr_size;
	_serverSocket = rhs._serverSocket;
	_clientSocket = rhs._clientSocket;
	inet_ntop(_client_addr.ss_family, get_addr((sockaddr *)&_client_addr), _inet, sizeof(_inet));
	return *this;
}

// member functions

void	*AUser::get_addr(sockaddr *saddr)
{
	if (saddr->sa_family == AF_INET)
		return &(((sockaddr_in *)saddr)->sin_addr);
	return &(((sockaddr_in6 *)saddr)->sin6_addr);
}

// getter

int	AUser::getClientSocket(void) const
{
	return _clientSocket;
}

char	*AUser::getInet(void)
{
	return _inet;
}

// setter

void	AUser::setClientSocket(void)
{
	if ((_clientSocket = accept(_serverSocket, (sockaddr *)&_client_addr, &_client_addr_size)) == -1) {
		throw Accept();
	}
	
	// get ipv4 or ipv6 address from client
	inet_ntop(_client_addr.ss_family, get_addr((sockaddr *)&_client_addr), _inet, sizeof(_inet));
}

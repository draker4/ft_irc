/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AUser.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:44:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/07 17:06:18 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AUser.hpp"

/* -----------------------------  Constructors  ----------------------------- */

AUser::AUser(void)
{
	// std::cout << GREEN << "AUser Default Constructor called "
	// 	<< RESET << std::endl;
}

AUser::AUser(int serverSocket) : _serverSocket(serverSocket), _client_addr_size(sizeof(_client_addr))
{
	// std::cout << GREEN << "AUser Constructor called "
	// 	<< RESET << std::endl;
}

AUser::AUser(const AUser &src)
{
	// std::cout << GREEN << "AUser Copy Constructor called "
	// 	<< RESET << std::endl;
	*this = src;
}

/* -----------------------------  Destructors  ------------------------------ */

AUser::~AUser(void)
{
	// std::cout << RED << "AUser Destructor called "
	// 	<< RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

AUser	&AUser::operator=(const AUser &rhs)
{
	// std::cout << GREEN << "AUser Assignment Operator called "
	// 	<< RESET << std::endl;
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

/* --------------------------------  Getter  -------------------------------- */

int	AUser::getClientSocket(void) const
{
	return _clientSocket;
}

char	*AUser::getInet(void)
{
	return _inet;
}

/* --------------------------------  Setter  -------------------------------- */

void	AUser::setClientSocket(void)
{
	if ((_clientSocket = accept(_serverSocket, (sockaddr *)&_client_addr, &_client_addr_size)) == -1) {
		throw Accept();
	}
	
	// get ipv4 or ipv6 address from client
	inet_ntop(_client_addr.ss_family, get_addr((sockaddr *)&_client_addr), _inet, sizeof(_inet));
}

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

void	*AUser::get_addr(sockaddr *saddr)
{
	if (saddr->sa_family == AF_INET)
		return &(((sockaddr_in *)saddr)->sin_addr);
	return &(((sockaddr_in6 *)saddr)->sin6_addr);
}

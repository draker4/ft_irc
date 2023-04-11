/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:44:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/10 15:17:08 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Client::Client(void)
{
	// std::cout << GREEN << "Client Default Constructor called "
	// 	<< RESET << std::endl;
}

Client::Client(int serverSocket) : _serverSocket(serverSocket), _nickname(""), \
_buffer(""), _client_addr_size(sizeof(_client_addr)), _registered(false), \
_deconnect(false)
{
	// std::cout << GREEN << "Client Constructor called "
	// 	<< RESET << std::endl;
}

Client::Client(const Client &src)
{
	// std::cout << GREEN << "Client Copy Constructor called "
	// 	<< RESET << std::endl;
	*this = src;
}

/* -----------------------------  Destructors  ------------------------------ */

Client::~Client(void)
{
	// std::cout << RED << "Client Destructor called "
	// 	<< RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Client &Client::operator=(const Client &rhs)
{
	// std::cout << GREEN << "Client Assignment Operator called "
	// 	<< RESET << std::endl;
	_nickname = rhs._nickname;
	_real_name = rhs._nickname;
	_username = rhs._username;
	_password = rhs._password;
	_client_addr = rhs._client_addr;
	_client_addr_size = rhs._client_addr_size;
	_serverSocket = rhs._serverSocket;
	_clientSocket = rhs._clientSocket;
	_buffer = rhs._buffer;
	inet_ntop(_client_addr.ss_family, get_addr((sockaddr *)&_client_addr), _inet, sizeof(_inet));
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

int Client::getClientSocket(void) const
{
	return _clientSocket;
}

char *Client::getInet(void)
{
	return _inet;
}

bool Client::getRegistered(void) const
{
	return _registered;
}

std::string	Client::getBuffer(void) const
{
	return _buffer;
}

std::string	Client::getNickname(void) const
{
	return _nickname;
}

bool	Client::getDeconnect(void) const
{
	return _deconnect;
}

/* --------------------------------  Setter  -------------------------------- */

void Client::setNickname(std::string nickname)
{
	_nickname = nickname;
}

int Client::setClientSocket(void)
{
	if ((_clientSocket = accept(_serverSocket, (sockaddr *)&_client_addr, &_client_addr_size)) == -1)
	{
		return EXIT_FAILURE;
	}

	// get ipv4 or ipv6 address from client
	inet_ntop(_client_addr.ss_family, get_addr((sockaddr *)&_client_addr), _inet, sizeof(_inet));
	return EXIT_SUCCESS;
}

void	Client::setDeconnect(bool boolean)
{
	_deconnect = boolean;
}

/* --------------------------  Private functions  --------------------------- */

void *Client::get_addr(sockaddr *saddr)
{
	if (saddr->sa_family == AF_INET)
		return &(((sockaddr_in *)saddr)->sin_addr);
	return &(((sockaddr_in6 *)saddr)->sin6_addr);
}

/* -----------------------  Public member functions  ------------------------ */

void	Client::addBuffer(std::string to_add)
{
	_buffer.append(to_add);
}

void	Client::clearBuffer(void)
{
	_buffer.clear();
	_buffer = "";
}

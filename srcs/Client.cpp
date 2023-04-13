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
	if (DEBUG_CLIENT)
		std::cout << GREEN << "Client Default Constructor called " << RESET << std::endl;
}

Client::Client(int serverSocket) : _serverSocket(serverSocket), _nickname(""),
_oldNickname(""), _real_name(""), _username(""), _buffer(""),
_client_addr_size(sizeof(_client_addr)), _registered(false),
_password_ok(false), _deconnect(false), _is_op(false)
{
	if (DEBUG_CLIENT)
		std::cout << GREEN << "Client Constructor called " << RESET << std::endl;
}

Client::Client(const Client &src)
{
	if (DEBUG_CLIENT)
		std::cout << GREEN << "Client Copy Constructor called " << RESET << std::endl;
	*this = src;
}

/* -----------------------------  Destructors  ------------------------------ */

Client::~Client(void)
{
	if (DEBUG_CLIENT)
		std::cout << RED << "Client Destructor called " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Client &Client::operator=(const Client &rhs)
{
	if (DEBUG_CLIENT)
		std::cout << GREEN << "Client Assignment Operator called " << RESET << std::endl;
	_nickname = rhs._nickname;
	_real_name = rhs._nickname;
	_username = rhs._username;
	_password_ok = rhs._password_ok;
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

bool	Client::getPassword(void) const
{
	return _password_ok;
}

bool	Client::getIsOperator(void) const
{
	return _is_op;
}

std::string	Client::getUsername(void) const
{
	return _username;
}

std::string	Client::getRealName(void) const
{
	return _real_name;
}

std::string	Client::getOldNickname(void) const
{
	return _oldNickname;
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

void	Client::setPassword(bool boolean)
{
	_password_ok = boolean;
}

void	Client::setUsername(std::string username)
{
	if (username.length() > USERLEN)
		username = username.substr(0, 10);
	_username = username;
}

void	Client::setRealName(std::string realname)
{
	_real_name = realname;
}

void	Client::setRegistered(bool boolean)
{
	_registered = boolean;
	
}

void	Client::setOldNickname(std::string old_nickname)
{
	_oldNickname = old_nickname;
}

void	Client::setIsOperator(bool boolean)
{
	_is_op = boolean;
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

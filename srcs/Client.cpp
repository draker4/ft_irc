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

#include "Channel.hpp"
#include "Client.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Client::Client(void)
{
	if (DEBUG_CLIENT)
		std::cout << GREEN << "Client Default Constructor called " << RESET << std::endl;
}

Client::Client(int serverSocket) : _serverSocket(serverSocket), _nickName(""),
_oldNickName(""), _realName(""), _userName(""), _buffer(""), _mode(""), _reason_leaving(""),
_client_addr_size(sizeof(_client_addr)), _registered(false),
_passwordSet(false), _deconnect(false)
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
	_serverSocket = rhs._serverSocket;
	_clientSocket = rhs._clientSocket;
	_nickName = rhs._nickName;
	_oldNickName = rhs._oldNickName;	
	_realName = rhs._nickName;
	_userName = rhs._userName;
	_buffer = rhs._buffer;
	_mode = rhs._mode;
	_reason_leaving = rhs._reason_leaving;
	_client_addr = rhs._client_addr;
	_client_addr_size = rhs._client_addr_size;
	inet_ntop(_client_addr.ss_family, _get_addr((sockaddr *)&_client_addr), _inet, sizeof(_inet));
	_registered = rhs._registered;
	_passwordSet = rhs._passwordSet;
	_deconnect = rhs._deconnect;
	_channels = rhs._channels;
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

std::string	Client::getNickName(void) const
{
	return _nickName;
}

bool	Client::getDeconnect(void) const
{
	return _deconnect;
}

bool	Client::getPasswordStatus(void) const
{
	return _passwordSet;
}

std::string	Client::getUserName(void) const
{
	return _userName;
}

std::string	Client::getRealName(void) const
{
	return _realName;
}

std::string	Client::getOldNickName(void) const
{
	return _oldNickName;
}

std::string	Client::getMode(void) const
{
	return _mode;
}

bool	Client::getModeStatus(char c) const
{
	for (itString it = _mode.begin(); it != _mode.end(); it++) {
		if (*it == c)
			return true;
	}
	return false;
}

Client::vecChannel	Client::getChannels() const
{
	return _channels;
}

std::string	Client::getReasonLeaving(void) const
{
	return _reason_leaving;
}

/* --------------------------------  Setter  -------------------------------- */

int Client::setClientSocket(void)
{
	if ((_clientSocket = accept(_serverSocket, (sockaddr *)&_client_addr,
		&_client_addr_size)) == -1) {
		return EXIT_FAILURE;
	}

	// get ipv4 or ipv6 address from client
	inet_ntop(_client_addr.ss_family, _get_addr((sockaddr *)&_client_addr),
		_inet, sizeof(_inet));
	return EXIT_SUCCESS;
}

void	Client::setDeconnect(bool boolean)
{
	_deconnect = boolean;
}

void	Client::setPasswordStatus(bool boolean)
{
	_passwordSet = boolean;
}

void Client::setNickName(std::string nickName)
{
	if (nickName.length() >= MAXNICKLEN)
		nickName = nickName.substr(0, MAXNICKLEN);
	_nickName = nickName;
}

void	Client::setUserName(std::string userName)
{
	if (userName.length() > USERLEN)
		userName = userName.substr(0, USERLEN);
	_userName = userName;
}

void	Client::setRealName(std::string realName)
{
	_realName = realName;
}

void	Client::setOldNickName(std::string oldNickName)
{
	_oldNickName = oldNickName;
}

void	Client::setRegistered(bool boolean)
{
	_registered = boolean;
}

void	Client::setReasonLeaving(std::string reason)
{
	_reason_leaving = reason;
}

/* --------------------------  Private functions  --------------------------- */

void	*Client::_get_addr(sockaddr *saddr)
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

void	Client::addMode(char c)
{
	if (_mode.find(c) == std::string::npos){
		_mode.push_back(c);
	}
}

void	Client::removeMode(char c)
{
	int i = 0;
	for (itString it = _mode.begin(); it != _mode.end(); it++) {
		if (*it == c) {
			_mode.erase(_mode.begin() + i);
			return;
		}
		i++;
	}
}

void	Client::addChannel(Channel *channel)
{
	if (channel)
		_channels.push_back(channel);
}

void	Client::removeChannel(Channel *channel)
{
	for (itVecChannel it = _channels.begin(); it != _channels.end(); it++) {
		if (*it == channel) {
			_channels.erase(it);
			return;
		}
	}
}

bool	Client::shareChannel(Client::vecChannel channels) const
{
	if (channels.empty() || _channels.empty())
		return false;
	
	// loop on all channels from client in argument
	for (constItVecChannel it_first = channels.begin(); it_first != channels.end(); it_first++) {

		// loop on all channels from this->client
		for (constItVecChannel it_second = _channels.begin(); it_second != _channels.end(); it_second++) {

			// check if same channel
			if (*it_first == *it_second) {
				return true;
			}
		}
	}
	return false;
}

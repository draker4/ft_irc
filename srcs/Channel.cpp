/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 19:13:35 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
# include "Client.hpp"
#include <sstream>


/* -----------------------------  Constructors  ----------------------------- */

Channel::Channel(void)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Default Constructor called " << RESET << std::endl;
}

Channel::Channel(std::string name, Client *client) : _symbol('='), _name(name), _mode("nt"),
	_topic(""), _clientLimit(0)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Constructor called with first client" << RESET << std::endl;
	addClient(client);
	_clients[client->getNickName()].userMode.push_back('q'); // q = owner
	_clients[client->getNickName()].prefix = '~'; // ~ = owner
	
	// Channel created
	std::stringstream timeChannel;
	timeChannel << static_cast< long long >( time(NULL) );
	_timeCreated = timeChannel.str();
}

Channel::Channel(const Channel &src)
{
	*this = src;
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Copy Constructor called " << RESET << std::endl;
}

/* -----------------------------  Destructors  ------------------------------ */

Channel::~Channel(void)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Default Destructor called " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Channel	&Channel::operator=(const Channel &rhs)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Assignment operator called " << RESET << std::endl;
	(void)rhs;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

std::string	Channel::getName(void) const
{
	return _name;
}

Channel::mapClients	Channel::getClients(void) const
{
	return _clients;
}

bool	Channel::getModeStatus(char c) const
{
	for (itConstString it = _mode.begin(); it != _mode.end(); it++) {
		if (*it == c)
			return true;
	}
	return false;
}

std::string	Channel::getKey(void) const
{
	return _key;
}

char	Channel::getSymbol(void) const
{
	return _symbol;
}

std::string	Channel::getPrefix(std::string nickName)
{
	itMapClients	it = _clients.find(nickName);
	if (it == _clients.end())
		return 0;
	return it->second.prefix;
}

std::string	Channel::getTopic(void) const
{
	return _topic;
}

std::string	Channel::getMode(void) const
{
	return _mode;
}

std::string	Channel::getTimeCreated(void)
{
	return _timeCreated;
}

std::string	Channel::getTimeTopic(void)
{
	return _timeLastTopic;
}

int	Channel::getOperGrade(std::string nickName)
{
	itMapClients	it = _clients.find(nickName);
	if (it == _clients.end())
		return 0;
	int	grade = 0;
	for (itString itUserMode = it->second.userMode.begin();
		itUserMode != it->second.userMode.end(); itUserMode++) {
		if (*itUserMode == 'q') // q = owner
			grade = 3;
		else if (*itUserMode == 'o' && grade < 2) // o = operator
			grade = 2;
		else if (*itUserMode == 'h' && grade < 1) // h = half operator
			grade = 1;
	}
	return grade;
}

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

void Channel::addClient(Client *client)
{
	t_connect	newClient;

	newClient.client = client;
	newClient.prefix = '\0';
	std::stringstream timeJoined;
	timeJoined << static_cast< long long >( time(NULL) );
	newClient.joinTime = timeJoined.str();
	_clients[client->getNickName()] = newClient;
}

bool	Channel::isBanned(std::string nickname)
{
	itMapMode	it = _banned.find(nickname);
	if (it == _banned.end())
		return false;
	return true;
}

bool	Channel::isFull(void) const
{
	return _clients.size() >= _clientLimit;
}

bool	Channel::isInvited(std::string nickname)
{
	itMapMode	it = _invited.find(nickname);
	if (it == _invited.end())
		return false;
	return true;
}

void	Channel::addBanned(std::string nickname)
{
	itMapClients	it = _clients.find(nickname);
	if (it == _clients.end())
		return ;
	std::stringstream timeBanned;
	timeBanned << static_cast< long long >( time(NULL) );
	_banned[nickname] = timeBanned.str();
}

void	Channel::addInvited(std::string nickname)
{
	itMapClients	it = _clients.find(nickname);
	if (it == _clients.end())
		return ;
	std::stringstream timeInvited;
	timeInvited << static_cast< long long >( time(NULL) );
	_invited[nickname] = timeInvited.str();
}

void	Channel::removeBanned(std::string nickname)
{
	itMapMode	it = _banned.find(nickname);
	if (it == _banned.end())
		return ;
	_banned.erase(it);
}

void	Channel::removeInvited(std::string nickname)
{
	itMapMode	it = _invited.find(nickname);
	if (it == _invited.end())
		return ;
	_invited.erase(it);
}

void	Channel::addMode(char c)
{
	if (_mode.find(c) == std::string::npos){
		_mode.push_back(c);
	}
}

void	Channel::removeMode(char c)
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

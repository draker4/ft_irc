/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/25 11:00:06 by bperriol         ###   ########lyon.fr   */
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
	_topic(""), _client_topic(""),_clientLimit(0)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Constructor called with first client" << RESET << std::endl;
	addClient(client);
	_clients[client->getNickName()].userMode.push_back('o'); // o = operator
	_clients[client->getNickName()].prefix = "@"; // @ = operator
	
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

bool	Channel::getUserModeStatus(std::string nickName, char c)
{
	itMapClients	it = _clients.find(nickName);
	if (it == _clients.end())
		return false;
	for (itConstString itUserMode = it->second.userMode.begin();
		itUserMode != it->second.userMode.end(); itUserMode++) {
		if (*itUserMode == c)
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
		if (*itUserMode == 'o') // o = operator
			grade = 3;
		else if (*itUserMode == 'h' && grade < 2) // h = half operator
			grade = 2;
		else if (*itUserMode == 'v' && grade < 1) // v = voice 
			grade = 1;
	}
	return grade;
}

std::string	Channel::getClientTopic(void) const
{
	return _client_topic;
}

unsigned int	Channel::getClientLimit(void) const
{
	return _clientLimit;
}

/* --------------------------------  Setter  -------------------------------- */

void	Channel::setTopic(std::string nickname, std::string topic)
{
	_topic = topic;
	_client_topic = nickname;
	std::stringstream timeTopic;
	timeTopic << static_cast< long long >( time(NULL) );
	_timeLastTopic = timeTopic.str();
}

void	Channel::setKey(std::string key)
{
	_key = key;
}

void	Channel::setClientLimit(std::string limit)
{
	_clientLimit = atoi(limit.c_str());
}

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

void	Channel::addClient(Client *client)
{
	t_connect	newClient;

	newClient.client = client;
	newClient.prefix = "";
	std::stringstream timeJoined;
	timeJoined << static_cast< long long >( time(NULL) );
	newClient.joinTime = timeJoined.str();
	_clients[client->getNickName()] = newClient;
}

void	Channel::removeClient(Client *client)
{
	itMapClients	it = _clients.find(client->getNickName());
	
	if (it != _clients.end())
		_clients.erase(it);
}

bool	Channel::isBanned(std::string nickname) const
{
	constItMapMode	it = _banned.find(nickname);
	if (it == _banned.end())
		return false;
	return true;
}

bool	Channel::isFull(void) const
{
	return _clients.size() >= _clientLimit;
}

bool	Channel::isInvited(std::string nickname) const
{
	constItMapMode	it = _invited.find(nickname);
	if (it == _invited.end())
		return false;
	return true;
}

bool	Channel::isClientInChannel(std::string nickname) const
{
	if (_clients.find(nickname) != _clients.end())
		return true;
	return false;
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

void	Channel::updateClient(std::string oldNickname, std::string nickname)
{
	itMapClients	it = _clients.find(oldNickname);
	
	if (it != _clients.end()) {
		t_connect	tmp = it->second;
		_clients.erase(it);
		_clients[nickname] = tmp;
	}
}

void	Channel::addUserMode(std::string nickname, char c)
{
	itMapClients	it = _clients.find(nickname);
	if (it == _clients.end())
		return ;
	if (it->second.userMode.find(c) == std::string::npos)
		it->second.userMode.push_back(c);
}

void	Channel::removeUserMode(std::string nickname, char c)
{
	itMapClients	it = _clients.find(nickname);
	if (it == _clients.end())
		return ;
	int i = 0;
	for (itString itUserMode = it->second.userMode.begin();
		itUserMode != it->second.userMode.end(); itUserMode++) {
		if (*itUserMode == c) {
			it->second.userMode.erase(it->second.userMode.begin() + i);
			return;
		}
		i++;
	}
}

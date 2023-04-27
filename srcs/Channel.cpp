/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/27 18:51:30 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
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
	_clients[toUpper(client->getNickName())].userMode.push_back('o'); // o = operator
	_clients[toUpper(client->getNickName())].prefix = "@"; // @ = operator
	
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
	itMapClients	it = _clients.find(toUpper(nickName));
	if (it == _clients.end())
		return "";
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
	itMapClients	it = _clients.find(toUpper(nickName));
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

Channel::mapBan	Channel::getBanList(void) const
{
	return _banned;
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
	_clients[toUpper(client->getNickName())] = newClient;
}

void	Channel::removeClient(Client *client)
{
	itMapClients	it = _clients.find(toUpper(client->getNickName()));
	
	if (it != _clients.end())
		_clients.erase(it);
}

bool	Channel::isBanned(Client *client) const
{
	// all posibilities :
	
	// nickname!*@*
	// *!*@host
	// *!user@* -> ya pas
	// nickname!user*
	// nickname!*@host
	// *user@host
	// nickname!user@host

	vecString	can_be_banned;
	
	can_be_banned.push_back(toUpper(client->getNickName()) + "!*@*");
	can_be_banned.push_back("*!*@" + toUpper(client->getInet()));
	can_be_banned.push_back("*!" + toUpper(client->getUserName()) + "@*");
	can_be_banned.push_back(toUpper(client->getNickName()) + "!" + toUpper(client->getUserName()) + "*");
	can_be_banned.push_back(toUpper(client->getNickName()) + "!*@" + toUpper(client->getInet()));
	can_be_banned.push_back("*" + toUpper(client->getUserName()) + "@" + toUpper(client->getInet()));
	can_be_banned.push_back(toUpper(client->getNickName()) + "!" + toUpper(client->getUserName()) + "@" + toUpper(client->getInet()));
	
	for (constItMapBan it = _banned.begin(); it != _banned.end(); it++) {
		for (itVecString it_string = can_be_banned.begin(); it_string != can_be_banned.end(); it_string++) {
			if (*it_string == it->first)
				return true;
		}
	}
	return false;
}

bool	Channel::isFull(void) const
{
	return _clients.size() >= _clientLimit;
}

bool	Channel::isClientInChannel(std::string nickname) const
{
	if (_clients.find(toUpper(nickname)) != _clients.end())
		return true;
	return false;
}

bool	Channel::addBanned(std::string ban, std::string banBy)
{
	if (ban.find_first_of("!") == std::string::npos && ban.find_first_of("@") == std::string::npos) {
		if (ban.find_first_of(".") == std::string::npos)
			ban += "!*@*";
		else {
			std::stringstream ss;
			ss << "*!*@" << ban ;
			ban = ss.str();
		}
	} else if (ban.find_first_of("@") == std::string::npos) {
		std::stringstream ss;
		ss << ban << "*";
		ban = ss.str();
	} else if (ban.find_first_of("!") == std::string::npos) {
		std::stringstream ss;
		ss << "*" << ban;
		ban = ss.str();
	}
	if (_banned.find(ban) != _banned.end())
		return false;
	std::stringstream timeBanned;
	timeBanned << static_cast< long long >( time(NULL) );
	_banned[ban].time = timeBanned.str();
	_banned[ban].banBy = banBy;
	return true;
}

bool	Channel::removeBanned(std::string ban)
{
	if (ban.find_first_of("!") == std::string::npos && ban.find_first_of("@") == std::string::npos) {
		if (ban.find_first_of(".") == std::string::npos)
			ban += "!*@*";
		else {
			std::stringstream ss;
			ss << "*!*@" << ban ;
			ban = ss.str();
		}
	} else if (ban.find_first_of("@") == std::string::npos) {
		std::stringstream ss;
		ss << ban << "*";
		ban = ss.str();
	} else if (ban.find_first_of("!") == std::string::npos) {
		std::stringstream ss;
		ss << "*" << ban;
		ban = ss.str();
	}
	itMapBan	it = _banned.find(ban);
	if (it == _banned.end())
		return (false);
	_banned.erase(it);
	return (true);
}

void	Channel::addInvited(std::string nickname)
{
	itMapInv	it = _invited.find(toUpper(nickname));
	if (it != _invited.end())
		return ;
	std::cout << "add invited" << std::endl;
	std::stringstream timeInvited;
	timeInvited << static_cast< long long >( time(NULL) );
	_invited[toUpper(nickname)] = timeInvited.str();
}

void	Channel::removeInvited(std::string nickname)
{
	itMapInv	it = _invited.find(toUpper(nickname));
	if (it == _invited.end())
		return ;
	_invited.erase(it);
}

bool	Channel::isInvited(std::string nickname)
{
	constItMapInv	it = _invited.find(toUpper(nickname));
	if (it == _invited.end())
		return false;
	this->removeInvited(toUpper(nickname));
	return true;
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
	itMapClients	it = _clients.find(toUpper(oldNickname));
	
	if (it != _clients.end()) {
		t_connect	tmp = it->second;
		_clients.erase(it);
		_clients[toUpper(nickname)] = tmp;
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
	itMapClients	it = _clients.find(toUpper(nickname));
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

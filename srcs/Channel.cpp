/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/13 18:11:47 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
# include "Client.hpp"


/* -----------------------------  Constructors  ----------------------------- */

Channel::Channel(void)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Default Constructor called " << RESET << std::endl;
}

Channel::Channel(std::string name, Client *client) : _name(name), _mode(""),
	_topic(""), _clientLimit(0)
{
	if (DEBUG_CHANNEL)
		std::cout << GREEN << "Channel Constructor called with first client" << RESET << std::endl;
	addClient(client);
	_clients[client->getNickName()].oper = 'q'; // q = owner
	
	// Server created
	_t_create = time(NULL);
	_t_create_str = ctime(&_t_create);
	_t_create_str = _t_create_str.substr(0, _t_create_str.length() - 1);
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

Channel::mapClients	Channel::getClients(void) const
{
	return _clients;
}

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

/* -----------------------  Public member functions  ------------------------ */

void Channel::addClient(Client *client)
{
	t_connect	newClient;

	newClient.client = client;
	newClient.oper = '\0';
	_clients.insert(std::pair<std::string, t_connect>(client->getNickName(), newClient));
}

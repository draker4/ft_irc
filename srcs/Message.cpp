/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:22:07 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/10 17:07:46 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Message::Message(void)
{}

Message::Message(std::string message) : _tags(""), _source(""), _command(""), _parameters("")
{
	size_t	pos = 0;
	size_t	prev = 0;

	if (message[0] == '@')
	{
		pos = message.find_first_of(" ");
		_tags = message.substr(0, pos - 1);
	}
	while (message[pos] == " ")
		pos++;
	if (message[pos] == ':')
	{
		prev = pos;
		pos = message.find_first_of(" ", pos);
		_source = message.substr(prev, pos - 1);
	}
	while (message[pos] == " ")
		pos++;
	prev = pos;
	if ((pos = message.find_first_of(" ")) != std::string::npos)
	{
		_command = message.substr(prev, pos - 1);
		while (message[pos] == " ")
			pos++;
		_parameters = message.substr(pos, message.find_first_of("\r\n") - 1);
	}
	else
		_command = message.substr(prev, message.find_first_of("\r\n") - 1);
}

Message::Message(const Message &src)
{
	*this = src;
}

/* -----------------------------  Destructors  ------------------------------ */

Message::~Message(void)
{
}

/* -------------------------  Assignment Operator  -------------------------- */

Message	&Message::operator=(const Message &rhs)
{
	_tags = rhs._tags;
	_source = rhs._source;
	_command = rhs._command;
	_parameters = rhs._parameters;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

std::string	Message::getTags(void) const
{
	return _tags;
}

std::string	Message::getSource(void) const
{
	return _source;
}

std::string	Message::getComman(void) const
{
	return _command;
}

std::string	Message::getParameters(void) const
{
	return _parameters;
}

/* --------------------------------  Setter  -------------------------------- */

/* ----------------------  Private member functions  ------------------------ */

/* -----------------------  Public member functions  ------------------------ */

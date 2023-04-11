/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:22:07 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/10 20:01:25 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Message::Message(void)
{}

Message::Message(std::string message) : _source("")
{
	std::vector<std::string>	vec = split(message, " ");

	if (vec.empty())
		throw ErrorMsgFormat();
	
	if (vec[0][0] == '@')
		_tags = split(first, ";")
	if (first[0] == ':' || (!_tags.empty() && vec[1][0] == ':'))
		//continue here

	size_t	pos = 0;
	size_t	prev = 0;
	


	if (message[0] == '@')
	{
		pos = message.find_first_of(" ");
		while (prev < pos)
		{
			size_t	tmp = pos;
			if (pos = message.find_first_of(";", prev) != std::string::npos && pos < tmp)
			{
				_tags.push_back(message.substr(prev, pos - 1));
				prev = pos + 1;
				if (message[prev] == ' ')
					throw ErrorMsgFormat();
			}
			else
			{
				_tags.push_back(message.substr(prev, tmp - 1));
				prev = tmp;
			}
			pos = tmp;
		}
	}
	while (message[pos] == ' ')
		pos++;
	if (message[pos] == ':')
	{
		prev = pos;
		pos = message.find_first_of(" ", pos);
		_source = message.substr(prev + 1, pos - 1);
		if (_tags.empty() && message[0] != ':')
			throw ErrorMsgFormat();
	}
	while (message[pos] == ' ')
		pos++;
	prev = pos;
	if ((pos = message.find_first_of(" ")) != std::string::npos)
	{
		_command = message.substr(prev, pos - 1);
		while (message[pos] == ' ')
			pos++;
		prev = pos;
		pos = message.find_first_of("\r\n");
		while (prev < pos)
		{
			size_t	tmp = pos;
			if (pos = message.find_first_of(" ", prev) != std::string::npos && pos < tmp)
			{
				_tags.push_back(message.substr(prev, pos - 1));
				prev = pos + 1;
				if (message[prev] == ' ')
					throw ErrorMsgFormat();
			}
				else
				{
					_tags.push_back(message.substr(prev, tmp - 1));
					prev = tmp;
				}
				pos = tmp;
			}
		}
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

std::vector<std::string>	Message::split(std::string message, std::string character)
{
	std::vector<std::string>	result;
	size_t						pos = 0;
	size_t						prev = pos;

	while ((pos = message.find_first_of(character, pos + 1)) != std::string::npos)
	{
		result.push_back(message.substr(prev, pos -1));
		prev = pos + 1;
	}
	pos = message.find_first_of("\r\n");
	result.push_back(message.substr(prev, pos - 1));
	return result;
}

/* -----------------------  Public member functions  ------------------------ */

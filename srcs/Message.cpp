/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:22:07 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/11 12:51:14 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Message::Message(void)
{}

Message::Message(std::string message) : _source("")
{
	if (message.empty() || message[0] == ' ')
		throw ErrorMsgFormat();

	vecString	vec = _split(message, " ");

	if (vec.empty())
		throw ErrorMsgFormat();
	
	// parse tags
	if (vec[0][0] == '@') {
		_tags = _split(vec[0], ";");
		_tags[0] = _tags[0].substr(1, _tags[0].length());
		vec.erase(vec.begin());
	}
	
	// parse source
	if (vec[0][0] == ':') {
		_source = vec[0];
		_source = _source.substr(1, _source.length());
		vec.erase(vec.begin());
	}
	
	// parse command
	_command = vec[0];
	vec.erase(vec.begin());
	for (itString it = _command.begin(); it != _command.end(); it++) {
		if (!isalnum(*it))
			throw ErrorMsgFormat();
	}
	
	// parse parameters
	for (itVecString it = vec.begin(); it != vec.end(); it++) {
		if ((*it)[0] == ':')
			(*it) = (*it).substr(1, (*it).length());
		_parameters.push_back(*it);
	}

	// print
	// for (std::vector<std::string>::iterator it = _tags.begin(); it != _tags.end(); it++) {
	// 	std::cout << "tags = " << *it << std::endl;
	// }
	// std::cout << "source = " << _source << std::endl;
	// std::cout << "command = " << _command << std::endl;
	// for (std::vector<std::string>::iterator it = _parameters.begin(); it != _parameters.end(); it++) {
	// 	std::cout << "parameters = " << *it << std::endl;
	// }
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

std::vector<std::string>	Message::getTags(void) const
{
	return _tags;
}

std::string	Message::getSource(void) const
{
	return _source;
}

std::string	Message::getCommand(void) const
{
	return _command;
}

std::vector<std::string>	Message::getParameters(void) const
{
	return _parameters;
}

/* --------------------------------  Setter  -------------------------------- */

/* ----------------------  Private member functions  ------------------------ */

std::vector<std::string>	Message::_split(std::string message, std::string character)
{
	std::vector<std::string>	result;
	bool						keep_spaces = false;
	size_t						pos = -1;
	size_t						prev = 0;

	while ((pos = message.find_first_of(character, pos + 1)) != std::string::npos)
	{
		if (keep_spaces && message.substr(prev, pos - prev)[0] == ':')
			break ;
		result.push_back(message.substr(prev, pos - prev));
		if (result.back()[0] != '@' && result.back()[0] != ':')
			keep_spaces = true;
		prev = pos + 1;
	}
	pos = message.find_first_of("\r\n");
	result.push_back(message.substr(prev, pos - prev));
	return result;
}

/* -----------------------  Public member functions  ------------------------ */

// int main(void)
// {
// 	std::string	str = "@ceci;est;un;tag :hahaouiiii allo coucou toi :ouahh ca     marche\r\n";
// 	// std::string	str = "PASS\r\n";
// 	try {
// 		Message	message(str);
// 	}
// 	catch(const std::exception &e) {
// 		std::cout << e.what() << std::endl;
// 	}
// }

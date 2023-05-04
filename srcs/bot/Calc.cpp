/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Calc.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 13:35:07 by bperriol          #+#    #+#             */
/*   Updated: 2023/05/04 12:10:56 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "Calc.hpp"

extern bool botOpen;

/* -----------------------------  Constructors  ----------------------------- */

Calc::Calc(void)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calc Default Constructor called " << RESET << std::endl;
}

Calc::Calc(std::string servername, std::string port, std::string password) : ABot(servername, port, password)
{
	if (DEBUG_CALCULATOR) {
		std::cout << GREEN << "Calc Constructor called servername and port"
		<< RESET << std::endl;
	}
}

Calc::Calc(const Calc &src) : ABot(src)
{
	*this = src;
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calc Copy Constructor called " << RESET << std::endl;
}


/* -----------------------------  Destructors  ------------------------------ */

Calc::~Calc(void)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calc Default Destructor called " << RESET << std::endl;
}

/* -------------------------  Assignment Operator  -------------------------- */

Calc	&Calc::operator=(const Calc &rhs)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calc Assignment operator called " << RESET << std::endl;
	(void)rhs;
	return *this;
}

/* -----------------------  Private member functions  -------------------------- */

std::string	Calc::_calc(std::string to_parse) const
{
	size_t	begin;
	size_t	end;
	size_t	pos = 0;
	double	result = 0;
	char	op = '\0';
	bool	number = false;

	if (to_parse.find_first_not_of("0123456789., +-/*") != std::string::npos)
		return ("I'm sorry, I can only understand these characters: '0123456789., +-/*'");
	
	while ((pos = to_parse.find_first_of(",")) != std::string::npos) {
		to_parse.replace(pos, 1, ".");
	}

	pos = 0;
	while (to_parse[pos]) {
		if (isdigit(to_parse[pos]) || to_parse[pos] == '.') {
			begin= pos;
			while (isdigit(to_parse[pos])
				|| (to_parse[pos] == '.' && to_parse[pos + 1] && to_parse[pos + 1] != '.'))
				pos++;
			end = pos--;
			double nb = strtod(to_parse.substr(begin, end - begin).c_str(), NULL);
			std::cout << "HERE:" << nb << std::endl;
			
			switch (op) {
				case '+':
					result += nb;
					break;
				case '-':
					result -= nb;
					break;
				case '*':
					result *= nb;
					break;
				case '/':
					if (nb == 0)
						return "Can't divide by 0, please try another calculation!";
					result /= nb;
					break;
				default:
					result += nb;
					break;
			}
			op = '\0';
			if (number == true)
				return "Format error";
			number = true;
		}
		else if (to_parse[pos] != ' ') {
			if (number == false)
				return ("Format error");
			number = false;
			op = to_parse[pos];
		}
		pos++;
	}

	std::stringstream	ss;
	ss << result;
	return ss.str();
}

/* -----------------------  Public member functions  ------------------------ */

void	Calc::bot_purpose(void) const
{
	Message		msg(_recvBuffer);
	std::string	send_to = "";
	
	if ((msg.getCommand() != "PRIVMSG" && msg.getCommand() != "NOTICE")
		|| msg.getParameters().size() < 2)
		return ;
	
	// get nickname or channel where the bot has to send his reply
	if (msg.getParameters()[0] == BOTNAME)
		send_to = msg.getSource().substr(0, msg.getSource().find_first_of("!"));
	else
		send_to = msg.getParameters()[0];

	std::string	to_parse = msg.getParameters()[1];
	
	std::string	reply = _calc(to_parse);
	
	sendServer(":" + BOTNAME + "!" + BOTNAME + "@" + _inet + " NOTICE " + send_to + " :" + reply + "\r\n");
}

void	Calc::joinCalcChannel(void) const
{
	sendServer(":" + BOTNAME + "!" + BOTNAME + "@" + _inet + " JOIN #calc\r\n");
}

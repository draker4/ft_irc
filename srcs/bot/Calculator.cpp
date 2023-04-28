/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 13:57:00 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Calculator.hpp"

extern bool botOpen;

/* -----------------------------  Constructors  ----------------------------- */

Calculator::Calculator(void)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Default Constructor called " << RESET << std::endl;
}

Calculator::Calculator(std::string port, std::string password) : _password(password)
{
	if (DEBUG_CALCULATOR) {
		std::cout << GREEN << "Calculator Constructor called with port and password"
		<< RESET << std::endl;
	}
	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos
		|| password.empty()) {
		throw Calculator::CalculatorException("Error: Wrong arg format!");
	}
	_port = strtod(port.c_str(), NULL);
	if (_port < 0 || _port > 65536) {
		throw Calculator::CalculatorException("Error: Port number should be comprised between 0 and 65536!");
	}
	struct sockaddr_in serv_addr;
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
		throw Calculator::CalculatorException("Error opening socket");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(_port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));
	if (connect(_sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		throw Calculator::CalculatorException("Error connecting to server");
}

Calculator::Calculator(const Calculator &src)
{
	*this = src;
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Copy Constructor called " << RESET << std::endl;
}

/* -----------------------------  Destructors  ------------------------------ */

Calculator::~Calculator(void)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Default Destructor called " << RESET << std::endl;
	close(_sockfd);
}

/* -------------------------  Assignment Operator  -------------------------- */

Calculator	&Calculator::operator=(const Calculator &rhs)
{
	if (DEBUG_CALCULATOR)
		std::cout << GREEN << "Calculator Assignment operator called " << RESET << std::endl;
	(void)rhs;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

void Calculator::_connexion(void)
{
	char	buffer[BUFFER];
	//sprintf(login, "PASS %s\r\nNICK MyBot\r\nUSER MyBot 0 * :My Bot\r\n", password);
	sendServer("PASS " + _password + "\r\nNICK " + BOTNAME + "\r\nUSER " + BOTNAME
		+ " 0 * :" + BOTNAME + "\r\n" , _sockfd);
	// Wait for NICK response
	while (botOpen) {
		memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(_sockfd, buffer, sizeof(buffer), 0);
		if (bytesReceived < 0 && botOpen)
			throw Calculator::CalculatorException("Error reading from socket");
		else if (bytesReceived == 0)
			throw Calculator::CalculatorException("Server disconnected");
		// Print received message
		if (DEBUG_CALCULATOR)
			std::cout << PURPLE << "Received message: " << buffer << std::endl;
		if (strstr(buffer, "004") != NULL) {
			// Welcome message received the connexion is sucessfull
			break;
		} else if (strstr(buffer, "431") != NULL) {
			throw Calculator::CalculatorException("No nickname given");
		} else if (strstr(buffer, "432") != NULL) {
			throw Calculator::CalculatorException("Invalid username");
		} else if (strstr(buffer, "433") != NULL) {
			throw Calculator::CalculatorException("Nickname already in use");
		} else if (strstr(buffer, "461") != NULL) {
			throw Calculator::CalculatorException("Not enough parameters (USER)");
		} else if (strstr(buffer, "462") != NULL) {
			throw Calculator::CalculatorException("Unauthorized command (PASS)");
		} else if (strstr(buffer, "464") != NULL) {
			throw Calculator::CalculatorException("Incorrect password");
		}
	}
	if (!botOpen)
		throw Calculator::CalculatorException("The bot has been stopped");
}

/* -----------------------  Public member functions  ------------------------ */

void Calculator::launch(void)
{
	//connect to server
	try {
		_connexion();
	}
	catch (const Calculator::CalculatorException &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		throw Calculator::CalculatorException("Error: Connexion failed!");
	}
	char	buffer[BUFFER];
	//Wait to receive messages from the server
	while (botOpen) {
		memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(_sockfd, buffer, sizeof(buffer), 0);
		if (bytesReceived < 0)
			throw Calculator::CalculatorException("Error reading from socket");
		else if (bytesReceived == 0)
			throw Calculator::CalculatorException("Server disconnected");
		// Print received message
		if (DEBUG_CALCULATOR)
			std::cout << PURPLE << "Received message: " << buffer << std::endl;
		// Check if the message is a PRIVMSG command
		if (strstr(buffer, "PRIVMSG") != NULL) {
			// Extract the sender and message from the PRIVMSG command
			char* sender = strtok(buffer, "!");
			char* message = strstr(buffer, " :");
			if (message != NULL) {
				// Remove the leading space character from the message
				message += 2;
				// Check if the message is a command to the bot
				if (strstr(message, "hello") != NULL) {
					// Respond with a greeting
					sendServer("PRIVMSG " + std::string(sender) + " :Hello!\r\n", _sockfd);
				} else {
					// Respond with a default message
					sendServer("PRIVMSG " + std::string(sender) + " :I don't understand!\r\n", _sockfd);
				}
			}
		}
	}
}

void	Calculator::sendServer(const std::string &msg, const int &clientSocket) const
{
	size_t	bytes_sent = 0;

	if (DEBUG_CALCULATOR)
		std::cout << PURPLE << BOLD << msg << RESET << std::endl;
	
	while (bytes_sent < msg.length()) {
		const int	len = send(clientSocket, &(msg.c_str())[bytes_sent], msg.length() - bytes_sent, 0);
		if (len < 0)
			throw CalculatorException("Error: Bot can't send all bytes!");
		bytes_sent += len;
	}
}

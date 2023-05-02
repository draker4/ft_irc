/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:15 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/28 13:57:00 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ABot.hpp"

extern	bool botOpen;

/* -----------------------------  Constructors  ----------------------------- */

ABot::ABot(void) : _servername(""), _port(0), _password("")
{
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Default Constructor called " << RESET << std::endl;
}

ABot::ABot(std::string servername, std::string port, std::string password) : _servername(servername), _port(port), _password(password), _registered(false)
{
	if (DEBUG_ABOT)
		std::cout << GREEN << "String ABot Constructor called " << RESET << std::endl;
	
	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos
		|| servername.empty() || password.empty()) {
		throw ABot::ABotException("Error: Wrong arg format!");
	}
	double	port_test = strtod(port.c_str(), NULL);
	if (port_test < 0 || port_test > 65536) {
		throw ABot::ABotException("Error: Port number should be comprised between 0 and 65536!");
	}
}

ABot::ABot(const ABot &src) : _servername(src._servername), _port(src._port)
{
	*this = src;
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Copy Constructor called " << RESET << std::endl;
}

/* -----------------------------  Destructors  ------------------------------ */

ABot::~ABot(void)
{
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Default Destructor called " << RESET << std::endl;
	close(_clientSocket);
}

/* -------------------------  Assignment Operator  -------------------------- */

ABot	&ABot::operator=(const ABot &rhs)
{
	if (DEBUG_ABOT)
		std::cout << GREEN << "ABot Assignment operator called " << RESET << std::endl;
	_clientSocket = rhs._clientSocket;
	_reuse = rhs._reuse;
	_registered = rhs._registered;
	return *this;
}

/* --------------------------------  Getter  -------------------------------- */

/* --------------------------------  Setter  -------------------------------- */

/* --------------------------  Private functions  --------------------------- */

void	*ABot::_get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void	ABot::_addRecvBuffer(std::string buffer)
{
	_recvBuffer.append(buffer);
}

void ABot::_receiveData(void)
{
	char	buffer[BUFFER];
	
	memset(buffer, 0, sizeof(buffer));
	int bytesReceived = recv(_clientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived < 0 && botOpen)
		throw ABot::ABotException("Error reading from socket");
	else if (bytesReceived == 0)
		throw ABot::ABotException("Server disconnected");

	// Print received message
	std::cout << RED << "Bot received:" << buffer << RESET << std::endl;

	// add bytes received to bot buffer
	_addRecvBuffer(std::string(buffer));

	// if full message received
	if (strstr(_recvBuffer.c_str(), "\r\n")
		&& _recvBuffer[_recvBuffer.length() - 2] == '\r'
		&& _recvBuffer[_recvBuffer.length() - 1] == '\n') {
		bot_purpose();
		_recvBuffer.clear();
	}
}

/* -----------------------  Public member functions  ------------------------ */

void	ABot::connectToServer(void)
{
	struct addrinfo	hints, *serverInfo, *indexInfo;
	int				addInfo;
	_reuse = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((addInfo = getaddrinfo(_servername.c_str(), _port.c_str(), &hints, &serverInfo)) != 0) {
		std::cerr << RED << gai_strerror(addInfo) << RESET << std::endl;
		throw ABot::ABotException("Error: GetAddrInfo function!");
	}

	// loop through all the results of getaddrinfo and bin the first possible
	for (indexInfo = serverInfo; indexInfo != NULL; indexInfo = indexInfo->ai_next) {
		// Create the socket
		if ((_clientSocket = socket(indexInfo->ai_family, indexInfo->ai_socktype,
			indexInfo->ai_protocol)) == -1) {
			std::cerr << RED << "ERROR: Cannot create socket!" << RESET << std::endl;
			continue;
		}

		// set options for the socket
		if (setsockopt(_clientSocket, SOL_SOCKET, SO_REUSEADDR, &_reuse, sizeof(int)) == -1) {
			close(_clientSocket);
			freeaddrinfo(serverInfo);
			throw ABot::ABotException("ERROR: Cannot set any option for the server socket!");
		}

		// Connect to the server
		if (connect(_clientSocket, indexInfo->ai_addr, indexInfo->ai_addrlen) == -1) {
			close(_clientSocket);
			std::cerr << RED << "ERROR: Cannot connect to server!" << RESET << std::endl;
			continue;
		}
		break;
	}

	if (!indexInfo) {
		freeaddrinfo(serverInfo);
		close(_clientSocket);
		throw ABot::ABotException("ERROR: Client failed to connect!");
	}

	inet_ntop(indexInfo->ai_family,
		_get_in_addr((struct sockaddr *)indexInfo->ai_addr),
		_inet, sizeof(_inet));

	freeaddrinfo(serverInfo);
}

void	ABot::createUser(void)
{
	char	buffer[BUFFER];

	//sprintf(login, "PASS %s\r\nNICK MyBot\r\nUSER MyBot 0 * :My Bot\r\n", password);
	sendServer("PASS " + _password + "\r\nNICK " + BOTNAME + "\r\nUSER " + BOTNAME
		+ " 0 * :" + BOTNAME + "\r\n");
	
	// Wait for NICK response
	while (botOpen && !_registered) {
		memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(_clientSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived < 0 && botOpen)
			throw ABot::ABotException("Error reading from socket");
		else if (bytesReceived == 0)
			throw ABot::ABotException("Server disconnected");

		// Print received message
		std::cout << buffer << std::endl;

		if (DEBUG_ABOT)
			std::cout << PURPLE << "Received message: " << buffer << std::endl;
		if (strstr(buffer, "005") != NULL) {
			// Welcome message received the connexion is sucessfull
			_registered = true;
			break;
		} else if (strstr(buffer, "431") != NULL) {
			throw ABot::ABotException("No nickname given");
		} else if (strstr(buffer, "432") != NULL) {
			throw ABot::ABotException("Invalid username");
		} else if (strstr(buffer, "433") != NULL) {
			throw ABot::ABotException("Nickname already in use");
		} else if (strstr(buffer, "461") != NULL) {
			throw ABot::ABotException("Not enough parameters (USER)");
		} else if (strstr(buffer, "462") != NULL) {
			throw ABot::ABotException("Unauthorized command (PASS)");
		} else if (strstr(buffer, "464") != NULL) {
			throw ABot::ABotException("Incorrect password");
		}
	}
	if (!botOpen) {
		sendServer("QUIT :Leaving\r\n");
		throw ABot::ABotException("The bot has been stopped");
	}
}

void	ABot::sendServer(const std::string &msg) const
{
	size_t	bytes_sent = 0;

	if (DEBUG_ABOT)
		std::cout << PURPLE << BOLD << msg << RESET << std::endl;
	
	while (bytes_sent < msg.length()) {
		const int	len = send(_clientSocket, &(msg.c_str())[bytes_sent], msg.length() - bytes_sent, 0);
		if (len < 0)
			throw ABotException("Error: Bot can't send all bytes!");
		bytes_sent += len;
	}
}

void	ABot::launch(void)
{
	pollfd	client_fd;

	client_fd.fd = _clientSocket;
	client_fd.events = POLLIN;

	while (botOpen) {
		int ret = poll(&client_fd, 1, -1);
		if (ret == -1 && botOpen) {
			std::cerr << "ERROR: Poll failed!" << std::endl;
			break;
		}

		// if no event on this socket
		if (!client_fd.revents)
			continue;
		
		// if event POLLIN
		else if (client_fd.revents & POLLIN) {
			_receiveData();
		}
	}
	sendServer("QUIT :Leaving\r\n");
}

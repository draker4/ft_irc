/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:34:21 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/10 15:57:28 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "Client.hpp"
#include "command.hpp"

#define MAX_CLIENTS 10

class Server
{
private:
	int _serverSocket;
	int _port;
	int _reuse;
	std::string _password;
	std::vector<pollfd> _fds;
	std::map<int, Client *> _clients;

	// Constructors
	Server(void);
	Server(const Server &src);

	// Assignment Operator
	Server &operator=(const Server &rhs);

	// Private member functions
	void addUser(std::vector<pollfd> &new_fds);
	void receive_data(std::vector<pollfd>::iterator &it);
	void handle_command(std::string msg, int clientSocket);

public:
	// Constructors
	Server(std::string port, std::string password);

	// Destructors
	~Server(void);

	// Getter
	int getServerSocket(void) const;
	Client *getUser(int clientSocket);

	// Setter

	// Public member functions
	void launch(void);
};

#endif

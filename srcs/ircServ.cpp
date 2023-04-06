/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 16:07:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/06 13:56:17 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ircServ.hpp"

int main(int argc, char **argv)
{
	
	// handle error ports between 1024 and 65535

	if (argc != 3) {
		std::cerr << RED << "ERROR: Wrong number of arguments!" << RESET << std::endl;
		return ARG_NB;
	}
	
	//create socket
	int 		serverSocket;
	addrinfo	hints, *res;
	
	// load up address structs with getaddrinfo()
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, argv[1], &hints, &res);

    // Create the socket
	serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (serverSocket == -1) {
		std::cerr << RED << "ERROR: Can't create socket!"
			<< RESET << std::endl;
		return SOCKET_CREATION;
	}

	// Bind the socket to the port
	if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1) {
		std::cerr << RED << "Can't bind to IP/Port!"
			<< RESET << std::endl;
		return -2;
	}

	// Listen for incoming connections
	if (listen(serverSocket, SOMAXCONN) == -1) {
		std::cerr << RED << "Can't listen!"
			<< RESET << std::endl;
		return -3;
	}
	
	// Accept incoming connections and handle them
	sockaddr_in	client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];
	
	int clientSocket = accept(serverSocket, (sockaddr *)&client, &clientSize);
	if (clientSocket == -1)
	{
		std::cerr << "Problem with client connecting!" << std::endl;
		return -4;
	}

	// close the listening socket
	
	//close(serverSocket);
	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);
	
	int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
	
	if (result)
	{
		std::cout << host << " connected on " << svc << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	}
	
	// while receiving, display messge, echo message
	
	char buf[4096];

	while (true)
	{
		// clear buffer
		
		memset(buf, 0, 4096);
		
		// wait for a message
		
		int bytesRecv = recv(clientSocket, buf, 4096, 0);
		if (bytesRecv == -1)
		{
			std::cerr << "There was a connection issue!" << std::endl;
			break ;
		}

		if (bytesRecv == 0)
		{
			std::cout << "The client disconnected!" << std::endl;
			break ;
		}
		
		// display message
		
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
		
		//resend message
		
		send(clientSocket, buf, bytesRecv + 1, 0);
	}
	
	//close socket
	
	close(clientSocket);
	
	return 0;
}

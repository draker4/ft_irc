/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 16:07:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/06 14:05:57 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ircServ.hpp"

bool serverOpen = true;

static void	handleSignal(int signal)
{
	std::cout << YELLOW << "Server is shutting down... " << signal << RESET << std::endl;
	serverOpen = false;
}

int main(int argc, char **argv)
{
	
	// handle error ports between 1024 and 65535

	if (argc != 3) {
		std::cerr << RED << "ERROR: Wrong number of arguments!"
			<< RESET << std::endl;
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
	
	signal(SIGINT, handleSignal);

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
	
	int clientSocket = accept(serverSocket, (sockaddr *)&client, &clientSize);
	if (clientSocket == -1)
	{
		std::cerr << "Problem with client connecting!" << std::endl;
		return -4;
	}
	std::cout << "Server started, listening on port " << argv[1] << std::endl;

    // Set up the pollfd structures
    pollfd fds[MAX_CLIENTS + 1]; // plus 1 for the listening socket
    memset(fds, 0, sizeof(fds));

    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    int nbClients = 0;
	
	while (serverOpen) {
        // Call poll()
        int ret = poll(fds, nbClients + 1, -1);
        if (ret == -1) {
            std::cerr << "ERROR: Poll failed!" << std::endl;
            break;
        }
        // Check for incoming connections
        if (fds[0].revents & POLLIN) {
            // Accept the connection
            int clientSocket = accept(serverSocket, NULL, NULL);
            if (clientSocket == -1) {
                std::cerr << "ERROR: Can't accept connection!" << std::endl;
                continue;
            }
            // Add the client to the fds array
            if (nbClients == MAX_CLIENTS) {
                std::cerr << "ERROR: Too many clients!" << std::endl;
                close(clientSocket);
            } else {
                nbClients++;
                fds[nbClients].fd = clientSocket;
                fds[nbClients].events = POLLIN;
                std::cout << "New client connected, fd = " << clientSocket << std::endl;
            }
        }
        // Check for incoming data on the client sockets
        for (int i = 1; i <= nbClients; i++) {
            if (fds[i].revents & POLLIN) {
                char buf[4096];
                memset(buf, 0, sizeof(buf));
                int bytesReceived = recv(fds[i].fd, buf, sizeof(buf), 0);
                if (bytesReceived == -1) {
                    std::cerr << "ERROR: Can't receive data from client!" << std::endl;
                } else if (bytesReceived == 0) {
                    std::cout << "Client disconnected, fd = " << fds[i].fd << std::endl;
					close(fds[i].fd);
                    nbClients--;
                    fds[i] = fds[nbClients + 1];
				} else {
					std::cout << "Received: " << std::string(buf, 0, bytesReceived) 
						<< "from : " << fds[i].fd << std::endl;
				}
			}
		}
	}
	// Close all client sockets
	for (int i = 0; i <= nbClients; i++) {
		close(fds[i].fd);
	}
	close(serverSocket);
	return 0;
}

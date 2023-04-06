/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 16:07:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/06 15:48:19 by baptiste         ###   ########lyon.fr   */
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
	if (argc != 3) {
		std::cerr << RED << "ERROR: Wrong number of arguments!"
			<< RESET << std::endl;
		return ARG_NB;
	}
	//create socket
	int serverSocket, portNumber;
    sockaddr_in serverAddress;
	
	signal(SIGINT, handleSignal);

    // Get the port number from the command line arguments
    portNumber = atoi(argv[1]);

    // Create the socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << RED << "ERROR: Can't create socket!"
			<< RESET << std::endl;
		return SOCKET_CREATION;
	}
	
	 // Set up the server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);
	
	// Bind the socket to the port
	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
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
	
	std::cout << "Server started, listening on port " << portNumber << std::endl;

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

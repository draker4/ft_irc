/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 16:07:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/06 15:51:38 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ircServ.hpp"

bool serverOpen = true;

static void	handleSignal(int signal)
{
	std::cout << YELLOW << "\nServer is shutting down... " << signal << RESET << std::endl;
	serverOpen = false;
}

static void	*get_addr(sockaddr *saddr)
{
	if (saddr->sa_family == AF_INET)
		return &(((sockaddr_in *)saddr)->sin_addr);
	return &(((sockaddr_in6 *)saddr)->sin6_addr);
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << RED << "Usage: ./ircServ <Port> <Password>"
			<< RESET << std::endl;
		return ARG_NB;
	}
	
	//set up the server adress struct
	addrinfo	hints, *serverInfo, *indexInfo;
	int			info, serverSocket;
	int			reuse = 1;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((info = getaddrinfo(NULL, argv[1], &hints, &serverInfo)) != 0) {
		std::cerr << RED << gai_strerror(info)
			<< RESET << std::endl;
		return GETADDRINFO;
	}

	// loop through all the results of getaddrinfo and bin the first possible
	for (indexInfo = serverInfo; indexInfo != NULL; indexInfo = indexInfo->ai_next)
	{
		// Create the socket
		if ((serverSocket = socket(indexInfo->ai_family, indexInfo->ai_socktype, indexInfo->ai_protocol)) == -1) {
			std::cerr << RED << "ERROR: Cannot create socket!"
				<< RESET << std::endl;
			continue ;
		}

		// set options for the socket
		if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
		{
			close(serverSocket);
			freeaddrinfo(serverInfo);
			std::cerr << RED << "ERROR: Cannot set any option for the server socket!"
				<< RESET << std::endl;
			return SOCKET_OPTION;
		}

		// Bind the socket to the port
		if (bind(serverSocket, indexInfo->ai_addr, indexInfo->ai_addrlen) == -1) {
			close(serverSocket);
			std::cerr << RED << "ERROR: Cannot bind to IP/Port!"
				<< RESET << std::endl;
			continue ;
		}

		break;
	}
	
	freeaddrinfo(serverInfo);

	if (!indexInfo)
	{
		close(serverSocket);
		std::cerr << RED << "ERROR: Server failed to bind to IP/Port!"
			<< RESET << std::endl;
		return SOCKET_BIND;
	}

	// Listen for incoming connections
	if (listen(serverSocket, SOMAXCONN) == -1) {
		close(serverSocket);
		std::cerr << RED << "Can't listen!"
			<< RESET << std::endl;
		return LISTEN;
	}

    // Set up the pollfd structures
    pollfd fds[MAX_CLIENTS + 1]; // plus 1 for the listening socket
    memset(fds, 0, sizeof(fds));
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    int nbClients = 0;
	signal(SIGINT, handleSignal);
	
	std::cout << YELLOW << "Server waiting for connections..." << RESET << std::endl;

	sockaddr_storage	client_addr;
	socklen_t			client_addr_size = sizeof(client_addr);
	int					clientSocket;
	char				s[INET6_ADDRSTRLEN];
	
	while (serverOpen) {
		
        // Call poll()
        int ret = poll(fds, nbClients + 1, -1);
        if (ret == -1 && serverOpen) {
            std::cerr << "ERROR: Poll failed!" << std::endl;
            break;
        }
		
        // Check for incoming connections
        if (fds[0].revents & POLLIN) {
			
            // Accept the connection
            //int clientSocket = accept(serverSocket, NULL, NULL);
            //if (clientSocket == -1) {
			if ((clientSocket = accept(serverSocket, (sockaddr *)&client_addr, &client_addr_size)) == -1) {
                std::cerr << "ERROR: Can't accept connection!" << std::endl;
                continue;
            }
			
			// get ipv4 or ipv6 address from client
			inet_ntop(client_addr.ss_family, get_addr((sockaddr *)&client_addr), s, sizeof(s));

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
			std::cout << YELLOW << "Server got connection from " << s << std::endl;
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
	return SUCCESS;
}

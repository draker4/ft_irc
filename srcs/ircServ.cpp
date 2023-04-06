/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 16:07:05 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/06 13:33:09 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ircServ.hpp"
using namespace std;

int main()
{
	//create socket

	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		cerr << "Can't create socket!" << endl;
		return -1;
	}
	
	//bind socket to a IP / Port

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(55000);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		cerr << "Can't bind to IP/Port!" << endl;
		return -2;
	}

	// Mark the socket for listening in

	if (listen(listening, SOMAXCONN) == -1)
	{
		cerr << "Can't listen!" << endl;
		return -3;
	}

	//accept a call
	
	sockaddr_in	client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];
	
	int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
	
	if (clientSocket == -1)
	{
		cerr << "Problem with client connecting!" << endl;
		return -4;
	}

	// close the listening socket
	
	close(listening);
	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);
	
	int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
	
	if (result)
	{
		cout << host << " connected on " << svc << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on " << ntohs(client.sin_port) << endl;
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
			cerr << "There was a connection issue!" << endl;
			break ;
		}

		if (bytesRecv == 0)
		{
			cout << "The client disconnected!" << endl;
			break ;
		}
		
		// display message
		
		cout << "Received: " << string(buf, 0, bytesRecv) << endl;
		
		//resend message
		
		send(clientSocket, buf, bytesRecv + 1, 0);
	}
	
	//close socket
	
	close(clientSocket);
	
	return 0;
}

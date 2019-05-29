/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 12.03.19
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include "Server.h"
#include "LogPrinter.h"
#include <errno.h>
#include <cstring>
#include <exception>

/*
* This file defines class Server, described in Server.h.
* See all methods documentation in header file.
*/

Server::Server(int port) : _server_sockfd(socket(AF_INET, SOCK_STREAM, 0)), _port(port)
{
	/* Initialization */
	struct sockaddr_in server_address;
	int res;

	/* Setting address */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(_port);

	/* Binding */
	res = bind(_server_sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
	if (res != 0) {
		throw std::runtime_error("Can't bind the socket");
	}
}

Server::~Server()
{
	close(_server_sockfd);
	LogPrinter::print("Server socket was closed");
}

void Server::start()
{
	listen(_server_sockfd, 5);
}

int Server::get_client()
{
	/* Initialization */
	char log_message[80];
	int client_sockfd;
	struct sockaddr_in client_address;
	unsigned int length = sizeof(client_address);
	fd_set inputs;
	struct timeval timeout;
	int select_res;
	int result = -1;

	/* Setting variables for select() */
	FD_ZERO(&inputs);
	FD_SET(_server_sockfd, &inputs);
	/* Timeout */
	timeout.tv_sec = 2;
	timeout.tv_usec = 500000;

	select_res = select(FD_SETSIZE, &inputs, (fd_set*)NULL, (fd_set*)NULL, &timeout);
	switch (select_res) {
	case 0:		// No input connection
		break;

	case -1:	// Select failed
		throw std::runtime_error("Select was interrupted");

	default:	// There is an input connection
		if (FD_ISSET(_server_sockfd, &inputs)) {
			client_sockfd = accept(_server_sockfd, (struct sockaddr*)&client_address,
						&length);
			if (client_sockfd != -1) {
				sprintf(log_message, "Connected to the client with ip %s",
					inet_ntoa(client_address.sin_addr));
				LogPrinter::print(log_message);
				result = client_sockfd;
			}
			else {
				throw std::runtime_error("Failed to accept connection with the client");
			}
		}
		break;
	}

	return result;
}

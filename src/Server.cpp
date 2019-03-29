/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 12.03.19
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include "Server.h"
#include "LogPrinter.h"
#include <errno.h>
#include <cstring>

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
		throw "Can't bind the socket";
	}
}

Server::~Server()
{

}

void Server::start()
{
	listen(_server_sockfd, 5);
}

int Server::get_client()
{
	char log_message[80];
	int client_sockfd;
	struct sockaddr_in client_address;
	unsigned int length = sizeof(client_address);

	client_sockfd = accept(_server_sockfd, (struct sockaddr*)&client_address, &length);
	if (client_sockfd != -1) {
		sprintf(log_message, "Connected to the client with ip %s",
			inet_ntoa(client_address.sin_addr));
		LogPrinter::print(log_message);
	}
	else {
		sprintf(log_message, "%s", strerror(errno));
		LogPrinter::error(log_message);
		throw "Failed to accept connection to the client";
	}

	return client_sockfd;
}

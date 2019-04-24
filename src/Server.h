/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 12.03.19
*/

#ifndef SERVER_H
#define SERVER_H

/*
* This file describes class Server.
* It opens socket and accepts clients.
*/

class Server {
private:
	int _server_sockfd;
	int _port;

public:
	/*
	* Server() - port number should be passed as a parameter.
	* Creates socket and bind it.
	* Can throw std::exception with "Can't bind the socket".
	*/
	Server(int port);

	~Server();

	/*
	* start() - start listening socket for clients.
	*/
	void start();

	/*
	* get_client() - establishes connection with client and returns client's socket
	* or -1 if timeout.
	* A "Failed to accept connection to the client" std::exception can be throwed.
	*/
	int get_client();
};

#endif // SERVER_H

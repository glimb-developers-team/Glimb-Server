/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 13.03.19
*/

#ifndef CLIENT_PROCESSOR_H
#define CLIENT_PROCESSOR_H

/*
* This file describes class ClientProcessor.
* It serves client in separate thread.
*/

#include <string>
#include "DbConnector.h"
#include "rapidjson/document.h"

class ClientProcessor {
private:
	DbConnector _db_connector;
	void _processing_client(int client_sockfd);
	void _register(int client_sockfd, rapidjson::Document &document);
	void _login(int client_sockfd, rapidjson::Document &document);

public:
	ClientProcessor();
	~ClientProcessor();

	void new_client(int client_sockfd);
};

#endif // CLIENT_PROCESSOR_H

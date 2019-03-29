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
	/*
	* _db_connector - object for working with MySQL database.
	*/
	DbConnector _db_connector;

	/*
	* _processing_client() - receives message from client, who was added with new_client() (See below).
	* It expects message in json format, so it checks for valid fileds in it.
	*/
	void _processing_client(int client_sockfd);

	/*
	* _register() - registers new user in database using object _db_connector.
	* info - should be a json Object with fields:
	*							"name",
	*							"last_name",
	*							"middle_name",
	*							"number",
	*							"password",
	*							"type",
	* 							"foreman_number".
	* Sends "ok" or "error" message in specific json format.
	* Errors can be described by "This user already exists" or "Unknown database error".
	*/
	void _register(int client_sockfd, rapidjson::Value &info);

	/*
	* _login() - checks given number and password with data in database.
	* Number and password are transmitted in info, which should be a json Object
	* with fields:
	*		"number",
	*		"password".
	*/
	void _login(int client_sockfd, rapidjson::Value &info);

public:
	ClientProcessor();
	~ClientProcessor();

	/*
	* new_client() - creates a new thread for client processing and detach it.
	*/
	void new_client(int client_sockfd);
};

#endif // CLIENT_PROCESSOR_H

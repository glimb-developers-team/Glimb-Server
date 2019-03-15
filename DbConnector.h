/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 14.03.19
*/

#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <string>
#include "mysql.h"

class DbConnector {
private:
	MYSQL *_conn_ptr;

public:
	DbConnector();
	~DbConnector();

	std::string register_new(std::string name, std::string last_name,
				std::string middle_name, std::string number,
				std::string password, std::string type,
				std::string foreman_number);

	int login(std::string number, std::string password);
};

#endif // DB_CONNECTOR_H

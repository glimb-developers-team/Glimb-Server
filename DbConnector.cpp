/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 15.03.19
*/

#define HOST		"localhost"
#define USER_NAME	"GlimbAdmin"
#define PASSWORD	"secret"
#define DB_NAME		"Glimb"

#define QUERY_SIZE	1024

#include "DbConnector.h"
#include "LogPrinter.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

/*
* This file defines class DbConnector, which is described in DbConnector.h.
* See all methods documentation in the header file.
*/

DbConnector::DbConnector()
{
	_conn_ptr = mysql_init(NULL);
	if (_conn_ptr == NULL) {
		LogPrinter::print(mysql_error(_conn_ptr));
		throw "MySQL initialization failed";
	}

	_conn_ptr = mysql_real_connect(_conn_ptr, HOST, USER_NAME, PASSWORD, DB_NAME, 0, NULL, 0);
	if (_conn_ptr == NULL) {
		LogPrinter::print(mysql_error(_conn_ptr));
		throw "MySQL connection failed";
	}

	LogPrinter::print("Successfully connected to the MySQL server");
}

DbConnector::~DbConnector()
{
	mysql_close(_conn_ptr);
	LogPrinter::print("Disconnested from the MySQL server");
}

std::string DbConnector::register_new(std::string name, std::string last_name,
			std::string middle_name, std::string number,
			std::string password, std::string type,
			std::string foreman_number)
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;

	/* Setting query */
	snprintf(query, QUERY_SIZE, "INSERT INTO user(Name, LastName, MiddleName,\
PhoneNumber, Password, Type, ForemanPhoneNumber) VALUES(\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")",
name.c_str(), last_name.c_str(), middle_name.c_str(), number.c_str(),
password.c_str(), type.c_str(), foreman_number.c_str());

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	if (res != 0) {	// Insert error
		snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		return error;
	}

	return "OK";
}

int DbConnector::login(std::string number, std::string password)
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error [QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;
	int return_value;

	/* Setting query */
	snprintf(query, QUERY_SIZE,
		"SELECT COUNT(PhoneNumber) FROM user WHERE PhoneNumber = \"%s\" AND Password = \"%s\"",
		number.c_str(), password.c_str());

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	if (res != 0) {	// Select error
		snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw "MySQL select failed";
	}

	/* Return result */
	mysql_res = mysql_use_result(_conn_ptr);
	sqlrow = mysql_fetch_row(mysql_res);
	return_value = atoi(sqlrow[0]);
	mysql_free_result(mysql_res);
	return return_value;
}

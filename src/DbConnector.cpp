/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 15.03.19
*/

#define HOST		"localhost"
#define USER_NAME	"GlimbAdmin"
#define PASSWORD	"secret"
#define DB_NAME		"GlimbRu"

#define QUERY_SIZE	1024

#include "DbConnector.h"
#include "LogPrinter.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <exception>

/*
* This file defines class DbConnector, which is described in DbConnector.h.
* See all methods documentation in the header file.
*/

DbConnector::DbConnector()
{
	char query[QUERY_SIZE];

	_conn_ptr = mysql_init(NULL);
	if (_conn_ptr == NULL) {
		LogPrinter::print(mysql_error(_conn_ptr));
		throw std::runtime_error( "MySQL initialization failed");
	}

	_conn_ptr = mysql_real_connect(_conn_ptr, HOST, USER_NAME, PASSWORD, DB_NAME, 0, NULL, 0);
	if (_conn_ptr == NULL) {
		LogPrinter::print(mysql_error(_conn_ptr));
		throw std::runtime_error( "MySQL connection failed");
	}

	snprintf(query, QUERY_SIZE, "SET NAMES \'CP1251\'");
	mysql_query(_conn_ptr, query);

	LogPrinter::print("Successfully connected to the MySQL server");
}

DbConnector::~DbConnector()
{
	mysql_close(_conn_ptr);
	LogPrinter::print("Disconnected from the MySQL server");
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

int DbConnector::login(std::string number, std::string password, std::string &name,
		std::string &last_name, std::string &middle_name, std::string &user_type,
		std::queue<std::string> &clients_queue)
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	/* Setting query */
	snprintf(query, QUERY_SIZE,
		"SELECT Name, LastName, MiddleName, Type FROM user WHERE PhoneNumber = \"%s\" AND Password = \"%s\";",
		number.c_str(), password.c_str());

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	if (res != 0) {	// Select error
		snprintf(error, QUERY_SIZE, "MySQL error: %s;", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw std::runtime_error( "MySQL select failed");
	}

	/* Return result */
	mysql_res = mysql_use_result(_conn_ptr);

	sqlrow = mysql_fetch_row(mysql_res);
	if (sqlrow == NULL) {
		return -1;
	}

	name = sqlrow[0];
	last_name = sqlrow[1];
	middle_name = sqlrow[2];
	user_type = sqlrow[3];

	mysql_free_result(mysql_res);

	if (user_type == "foreman") {
		snprintf(query, QUERY_SIZE, "SELECT PhoneNumber FROM user WHERE ForemanPhoneNumber = \"%s\";", number.c_str());

		res = mysql_query(_conn_ptr, query);
		if (res != 0) {	// Select error
			snprintf(error, QUERY_SIZE, "MySQL error: %s;", mysql_error(_conn_ptr));
			LogPrinter::print(error);
			throw std::runtime_error( "MySQL select failed");
		}

		mysql_res = mysql_use_result(_conn_ptr);
		while ((sqlrow = mysql_fetch_row(mysql_res)) != NULL) {
			clients_queue.push(sqlrow[0]);
		}
	}

	mysql_free_result(mysql_res);
	return 0;
}

std::queue<material> DbConnector::get_materials()
{
	/* Initialization */
	std::queue<material> materials_queue;
	material tmp;
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	/* Setting query */
	snprintf(query, QUERY_SIZE,
		"SELECT * FROM material;");

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	if (res != 0) {	// Select error
		snprintf(error, QUERY_SIZE, "MySQL error: %s;", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw std::runtime_error( "MySQL select failed");
	}

	/* Return result */
	mysql_res = mysql_use_result(_conn_ptr);

	sqlrow = mysql_fetch_row(mysql_res);
	while (sqlrow != NULL) {
		tmp.title = sqlrow[0];
		tmp.unions = sqlrow[1];
		tmp.price = atof(sqlrow[2]);

		materials_queue.push(tmp);
		sqlrow = mysql_fetch_row(mysql_res);
	}

	mysql_free_result(mysql_res);
	return materials_queue;
}

void DbConnector::store_purchase(std::string foreman_num, std::string client_num, std::queue<purchase> purchases_queue)
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int purchase_id;
	purchase p;
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	snprintf(query, QUERY_SIZE, "INSERT INTO purchase(ClientNum, ForemanNum) VALUES(\"%s\", \"%s\")", client_num.c_str(), foreman_num.c_str());

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	if (res != 0) {	// Insert error
		snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw std::runtime_error( "MySQL insert failed");
	}

	mysql_res = mysql_use_result(_conn_ptr);
	mysql_free_result(mysql_res);

	snprintf(query, QUERY_SIZE, "SELECT LAST_INSERT_ID()");

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	if (res != 0) {	// Insert error
		snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw std::runtime_error( "MySQL insert failed");
	}

	mysql_res = mysql_use_result(_conn_ptr);
	sqlrow = mysql_fetch_row(mysql_res);
	purchase_id = atoi(sqlrow[0]);
	mysql_free_result(mysql_res);

	/* Pushing data to the storage */
	while (!purchases_queue.empty()) {
		p = purchases_queue.front();
		snprintf(query, QUERY_SIZE, "INSERT INTO purchase_material(PurchaseId, MaterialTitle, Quantity) VALUES(\"%d\", \"%s\", \"%d\")", purchase_id, p.title.c_str(), p.quantity);

		/* Sending query */
		res = mysql_query(_conn_ptr, query);
		if (res != 0) {	// Insert error
			snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
			LogPrinter::print(error);
			throw std::runtime_error( "MySQL insert failed");
		}
		purchases_queue.pop();
	}
}

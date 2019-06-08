/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 08.06.19
*/

#include "DbQueryHandlerBase.h"
#include "LogPrinter.h"
#include <cstdio>

#define HOST		"localhost"
#define USER_NAME	"GlimbAdmin"
#define PASSWORD	"secret"
#define DB_NAME		"GlimbRu"

#define QUERY_SIZE	1024

using namespace DbConnector::DbQueryHandler;

MYSQL *DbQueryHandlerBase::_conn_ptr = mysql_init(NULL);

DbQueryHandlerBase::DbQueryHandlerBase()
{
	char query[QUERY_SIZE];

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

DbQueryHandlerBase::~DbQueryHandlerBase()
{
	mysql_close(_conn_ptr);
	LogPrinter::print("Disconnected from the MySQL server");
}

int DbQueryHandlerBase::_mysql_query(const char *query)
{
	return mysql_query(_conn_ptr, query);
}

MYSQL_RES* DbQueryHandlerBase::_mysql_use_result()
{
	return mysql_use_result(_conn_ptr);
}

MYSQL_ROW DbQueryHandlerBase::_mysql_fetch_row(MYSQL_RES *mysql_res)
{
	return mysql_fetch_row(mysql_res);
}

void DbQueryHandlerBase::_mysql_free_result(MYSQL_RES *mysql_res)
{
	mysql_free_result(mysql_res);
}

void DbQueryHandlerBase::_check_for_error(int res)
{
	char error[QUERY_SIZE];

	if (res != 0) {	// MySQL error
		snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw std::runtime_error("MySQL query failed");
	}
}

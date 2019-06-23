/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 22.06.19
*/

#include "LoginDbQueryHandler.h"
#include "LoginDbArg.h"
#include "LoginDbResult.h"

using namespace DbConnector::DbQueryHandler;

DbConnector::DbResult::DbResult LoginDbQueryHandler::handle(DbArg::DbArg &arg)
{
	/* Initialization */
	DbArg::LoginDbArg &log_arg = static_cast<DbArg::LoginDbArg&>(arg);
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;
	DbResult::LoginDbResult log_res;

	/* Setting query */
	snprintf(query, QUERY_SIZE,
		"SELECT Name, LastName, MiddleName, Type, ForemanPhoneNumber FROM user WHERE PhoneNumber = \"%s\" AND Password = \"%s\";",
		log_arg.number.c_str(), log_arg.password.c_str());

	/* Sending query */
	res = _mysql_query(query);
	_check_for_error(res);

	/* Return result */
	mysql_res = _mysql_use_result();

	sqlrow = _mysql_fetch_row(mysql_res);
	if (sqlrow == NULL) {
		return log_res;	// Returning empty result
	}

	log_res.name = sqlrow[0];
	log_res.last_name = sqlrow[1];
	log_res.middle_name = sqlrow[2];
	log_res.type = sqlrow[3];
	log_res.foreman_number = sqlrow[4];

	_mysql_free_result(mysql_res);

	if (log_res.type == "foreman") {
		snprintf(query, QUERY_SIZE, "SELECT PhoneNumber FROM user WHERE ForemanPhoneNumber = \"%s\";",
			 log_arg.number.c_str());

		res = _mysql_query(query);
		_check_for_error(res);

		mysql_res = _mysql_use_result();
		while ((sqlrow = _mysql_fetch_row(mysql_res)) != NULL) {
			log_res.clients.push(sqlrow[0]);
		}
		_mysql_free_result(mysql_res);
	}

	return log_res;
}
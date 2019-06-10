/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 08.06.19
*/

#include "RegistrationDbQueryHandler.h"
#include "RegistrationDbArg.h"
#include "RegistrationDbResult.h"
#include <cstdio>

using namespace DbConnector::DbQueryHandler;

DbConnector::DbResult::DbResult RegistrationDbQueryHandler::handle(DbArg::DbArg &arg)
{
	/* Initialization */
	DbArg::RegistrationDbArg &reg_arg = static_cast<DbArg::RegistrationDbArg&>(arg);
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;

	/* Setting query */
	snprintf(query, QUERY_SIZE, "INSERT INTO user(Name, LastName, MiddleName,\
PhoneNumber, Password, Type, ForemanPhoneNumber)\
VALUES(\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")",
reg_arg.name.c_str(), reg_arg.last_name.c_str(), reg_arg.middle_name.c_str(),
reg_arg.number.c_str(), reg_arg.password.c_str(), reg_arg.type.c_str(),
reg_arg.foreman_number.c_str());

	/* Sending query */
	res = _mysql_query(query);
	_check_for_error(res);

	DbResult::RegistrationDbResult reg_res = DbResult::RegistrationDbResult("OK");
	return reg_res;
}

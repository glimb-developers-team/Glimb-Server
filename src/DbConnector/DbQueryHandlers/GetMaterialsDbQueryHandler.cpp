/*
* Author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 24.06.19
*/

#include "GetMaterialsDbQueryHandler.h"
#include "GetMaterialsDbArg.h"
#include "GetMaterialsDbResult.h"

using namespace DbConnector::DbQueryHandler;

DbConnector::DbResult::DbResult GetMaterialsDbQueryHandler::handle(DbArg::DbArg &arg)
{
	/* Initialization */
	DbArg::GetMaterialsDbArg &getmat_arg = static_cast<DbArg::GetMaterialsDbArg&>
					       (arg);
	std::queue<DbResult::Material> materials_queue;
	char query[QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	/* Setting query */
	snprintf(query, QUERY_SIZE, "SELECT * FROM material;");

	/* Sending query */
	res = _mysql_query(query);
	_check_for_error(res);

	/* Geting result */
	mysql_res = _mysql_use_result();

	/* Pushing materials to the queue */
	sqlrow = _mysql_fetch_row(mysql_res);
	while (sqlrow != NULL) {
		materials_queue.push(DbResult::Material(sqlrow[0],
							sqlrow[1],
							atof(sqlrow[2])));
		sqlrow = _mysql_fetch_row(mysql_res);
	}

	/* Returning result */
	_mysql_free_result(mysql_res);
	return DbResult::GetMaterialsDbResult(materials_queue);
}

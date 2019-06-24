/*
* Author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 24.06.19
*/

#ifndef GET_MATERIALS_DB_QUERY_HANDLER_H
#define GET_MATERIALS_DB_QUERY_HANDLER_H

#include "DbQueryHandlerBase.h"
#include "DbArg.h"
#include "DbResult.h"

namespace DbConnector::DbQueryHandler {
	/*
	* GetMaterialsDbQueryHandler - handler class, which returns
	* all materials form Db.
	*/
	class GetMaterialsDbQueryHandler : DbQueryHandlerBase {
	public:
		DbResult::DbResult handle(DbArg::DbArg &arg) override;
	};
}

#endif // GET_MATERIALS_DB_QUERY_HANDLER_H

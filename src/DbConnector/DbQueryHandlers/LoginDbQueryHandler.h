/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 22.06.19
*/

#ifndef LOGIN_DB_QUERY_HANDLER_H
#define LOGIN_DB_QUERY_HANDLER_H

#include "DbQueryHandlerBase.h"
#include "DbArg.h"
#include "DbResult.h"

namespace DbConnector::DbQueryHandler {
	/*
	* LoginDbQueryHandler - handler class, which provides
	* checking user for being registred in the system.
	*/
        class LoginDbQueryHandler : DbQueryHandlerBase {
	public:
		DbResult::DbResult handle(DbArg::DbArg &arg) override;
        };
}

#endif // LOGIN_DB_QUERY_HANDLER_H

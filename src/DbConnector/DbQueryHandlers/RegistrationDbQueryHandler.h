/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 08.06.19
*/

#ifndef REGISTRATION_DB_QUERY_HANDLER_H
#define REGISTRATION_DB_QUERY_HANDLER_H

#include "DbQueryHandlerBase.h"
#include "DbArg.h"
#include "DbResult.h"

namespace DbConnector::DbQueryHandler {
	/*
	* RegistrationDbQueryHandler - handler class, which
	* provides storing user registration data in the Db.
	*/
	class RegistrationDbQueryHandler : DbQueryHandlerBase {
	public:
		DbResult::DbResult handle(DbArg::DbArg &arg) override;
	};
}

#endif // REGISTRATION_DB_QUERY_HANDLER_H

/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#ifndef LOGIN_DB_RESULT_H
#define LOGIN_DB_RESULT_H

#include "DbResult.h"

namespace DbConnector::DbResult {
	/*
	* LoginDbResult - stuct which should be given
	* as an output to the relevant handler.
	*/
	struct LoginDbResult : DbResult {
		int result;

		LoginDbResult(int result);
	};
}

#endif // LOGIN_DB_RESULT_H

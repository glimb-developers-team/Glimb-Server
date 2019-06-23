/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#ifndef LOGIN_DB_RESULT_H
#define LOGIN_DB_RESULT_H

#include "DbResult.h"
#include <string>
#include <queue>

namespace DbConnector::DbResult {
	/*
	* LoginDbResult - stuct which should be given
	* as an output to the relevant handler.
	*/
	struct LoginDbResult : DbResult {
		std::string name;
		std::string last_name;
		std::string middle_name;
		std::string type;
		std::queue<std::string> clients;
		std::string foreman_number;

		LoginDbResult();

		LoginDbResult(std::string name, std::string last_name,
			      std::string middle_name, std::string type,
			      std::queue<std::string> clients,
			      std::string foreman_number);
	};
}

#endif // LOGIN_DB_RESULT_H

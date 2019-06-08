/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#ifndef LOGIN_DB_ARG_H
#define LOGIN_DB_ARG_H

#include "DbArg.h"
#include <string>
#include <queue>

namespace DbConnector::DbArg {
	/*
	* LoginDbArg - stuct which should be given
	* as an argument to the relevant handler.
	*/
	struct LoginDbArg : DbArg {
		std::string number;
		std::string password;
		std::string &name;
		std::string &last_name;
		std::string &middle_name;
		std::string &type;
		std::queue<std::string> &clients;
		std::string &foreman_number;

		LoginDbArg(std::string number, std::string password, std::string &name,
			std::string &last_name, std::string &middle_name, std::string &type,
			std::queue<std::string> &clients, std::string &foreman_number);
	};
}

#endif // LOGIN_DB_ARG_H

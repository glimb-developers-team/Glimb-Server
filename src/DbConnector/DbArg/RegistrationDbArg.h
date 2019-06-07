/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "DbArg.h"
#include <string>

namespace DbConnector::DbArg {
	/*
	* RegistrationDbArg - stuct which should be given
	* as an argument to the relevant handler.
	*/
	struct RegistrationDbArg : DbArg {
		std::string name;
		std::string last_name;
		std::string middle_name;
		std::string number;
		std::string password;
		std::string type;
		std::string foreman_number;

		RegistrationDbArg(std::string name, std::string last_name, std::string middle_name,
				std::string number, std::string password, std::string type,
				std::string foreman_number);
	};
}

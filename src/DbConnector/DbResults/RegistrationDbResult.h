/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "DbResult.h"
#include <string>

namespace DbConnector::DbResult {
	/*
	* RegistrationDbResult - struct which should be given
	* as an output to the relevant handler.
	*/
	struct RegistrationDbResult : DbResult {
		std::string result;

		RegistrationDbResult(std::string result);
	};
}

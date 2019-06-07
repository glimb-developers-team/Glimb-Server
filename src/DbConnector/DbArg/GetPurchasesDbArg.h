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
	* GetMaterialsDbArg - stuct which should be given
	* as an argument to the relevant handler.
	*/
	struct GetPurchasesDbArg : DbArg {
		std::string client_number;
		std::string foreman_number;

		GetPurchasesDbArg(std::string client_number, std::string foreman_number);
	};
}

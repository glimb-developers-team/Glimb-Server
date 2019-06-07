/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "DbArg.h"
#include "DbResult.h"
#include <string>
#include <queue>

namespace DbConnector::DbArg {
	typedef DbResult::SelectedMaterial SelectedMaterial;

	/*
	* StorePurchaseDbArg - stuct which should be given
	* as an argument to the relevant handler.
	*/
	struct StorePurchaseDbArg : DbArg {
		std::string client_number;
		std::string foreman_number;
		std::queue<SelectedMaterial> materials;

		StorePurchaseDbArg(std::strings client_number, std::string foreman_number,
				std::queue<SelectedMaterial> materials);
	};
}

/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#ifndef STORE_PURCHASE_DB_ARG_H
#define STORE_PURCHASE_DB_ARG_H

#include "DbArg.h"
#include "GetPurchasesDbResult.h"
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

		StorePurchaseDbArg(std::string client_number, std::string foreman_number,
				std::queue<SelectedMaterial> materials);
	};
}

#endif // STORE_PURCHASE_DB_ARG_H

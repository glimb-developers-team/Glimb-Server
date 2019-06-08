/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#ifndef GET_PURCHASES_DB_RESULT_H
#define GET_PURCHASES_DB_RESULT_H

#include "DbResult.h"
#include <queue>
#include <string>

namespace DbConnector::DbResult {
	enum Status {
		STATUS_FALSE,
		STATUS_TRUE,
		STATUS_NULL
	};

	struct SelectedMaterial {
		std::string title;
		double quantity;
		double cost;

		SelectedMaterial(std::string title, double quantity, double cost);
	};

	struct PurchaseToSend {
		int id;
		std::queue<SelectedMaterial> materials;
		double total_cost;
		Status status;

		PurchaseToSend(int id, std::queue<SelectedMaterial> materials,
			double total_cost, Status status);
	};

	/*
	* GetPurchasesDbResult - struct which should be given
	* as an output to the relevant handler.
	*/
	struct GetPurchasesDbResult : DbResult {
		std::queue<PurchaseToSend> purchases;

		GetPurchasesDbResult(std::queue<PurchaseToSend> materials);
	};
}

#endif // GET_PURCHASES_DB_RESULT_H

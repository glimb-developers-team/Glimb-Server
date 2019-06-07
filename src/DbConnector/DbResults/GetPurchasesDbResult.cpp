/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "GetPurchasesDbResult.h"

using namespace DbConnector::DbResult;

SelectedMaterial::SelectedMaterial(std::string title, double quantity, double cost) :
			title(title), quantity(quantity), cost(cost)
{

}

PurchaseToSend::PurchaseToSend(int id, std::queue<SelectedMaterial> materials,
			double total_cost, Status status) :
			id(id), materials(materials), total_cost(total_cost), status(status)
{

}

GetPurchasesDbResult::GetPurchasesDbResult(std::queue<PurchaseToSend> purchases) : purchases(purchases)
{

}

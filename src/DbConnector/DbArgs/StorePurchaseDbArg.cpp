/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "StorePurchaseDbArg.h"

using namespace DbConnector::DbArg;

StorePurchaseDbArg::StorePurchaseDbArg(std::string client_number, std::string foreman_number,
		std::queue<SelectedMaterial> materials) :
		client_number(client_number), foreman_number(foreman_number), materials(materials)
{

}

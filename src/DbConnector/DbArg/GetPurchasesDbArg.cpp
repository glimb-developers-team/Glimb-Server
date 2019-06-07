/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "GetPurchasesDbArg.h"

using namespace DbConnector::DbArg;

GetPurchasesDbArg::GetPurchasesDbArg(std::string client_number, std::string foreman_number) :
				client_number(client_number), foreman_number(foreman_number)
{

}

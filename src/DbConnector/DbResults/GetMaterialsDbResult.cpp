/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "GetMaterialsDbResult.h"

using namespace DbConnector::DbResult;

Material::Material(std::string title, std::string unions, double price) :
			title(title), unions(unions), price(price)
{

}

GetMaterialsDbResult::GetMaterialsDbResult(std::queue<Material> materials) : materials(materials)
{

}

/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#ifndef GET_MATERIALS_DB_RESULT_H
#define GET_MATERIALS_DB_RESULT_H

#include "DbResult.h"
#include <queue>
#include <string>

namespace DbConnector::DbResult {
	struct Material {
		std::string title;
		std::string unions;
		double price;

		Material(std::string title, std::string unions, double price);
	};

	/*
	* GetMaterialsDbResult - struct which should be given
	* as an output to the relevant handler.
	*/
	struct GetMaterialsDbResult : DbResult {
		std::queue<Material> materials;

		GetMaterialsDbResult(std::queue<Material> materials);
	};
}

#endif // GET_MATERIALS_DB_RESULT_H

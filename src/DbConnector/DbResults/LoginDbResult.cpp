/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "LoginDbResult.h"

using namespace DbConnector::DbResult;

LoginDbResult::LoginDbResult(std::string name, std::string last_name,
			     std::string middle_name, std::string type,
			     std::queue<std::string> clients,
			     std::string foreman_number) :
			     number(number), password(password), name(name),
			     last_name(last_name), middle_name(middle_name),
			     type(type), clients(clients),
			     foreman_number(foreman_number)
{

}

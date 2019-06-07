/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "LoginDbArg.h"

using namespace DbConnector::DbArg;

LoginDbArg::LoginDbArg(std::string number, std::string password, std::string &name,
	std::string &last_name, std::string &middle_name, std::string &type,
	std::queue<std::string> &clients, std::string &foreman_number) :
	number(number), password(password), name(name), last_name(last_name),
	middle_name(middle_name), type(type), clients(clients), foreman_number(foreman_number)
{

}

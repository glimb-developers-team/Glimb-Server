/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 07.06.19
*/

#include "RegistrationDbArg.h"

using namespace DbConnector::DbArg;

RegistrationDbArg::RegistrationDbArg(std::string name, std::string last_name, std::string middle_name,
			std::string number, std::string password, std::string type,
			std::string foreman_number) :
			name(name), last_name(last_name), middle_name(middle_name), number(number),
			password(password), type(type), foreman_number(foreman_number)
{

}

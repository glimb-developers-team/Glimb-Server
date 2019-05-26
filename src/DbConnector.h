/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 14.03.19
*/

#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <string>
#include <queue>
#include "mysql.h"

struct material {
	std::string title;
	std::string unions;
	double price;
};

struct purchase {
	std::string title;
	int quantity;
};

class DbConnector {
private:
	MYSQL *_conn_ptr;
	int get_new_purchase_localid();

public:
	DbConnector();
	~DbConnector();

	std::string register_new(std::string name, std::string last_name,
				std::string middle_name, std::string number,
				std::string password, std::string type,
				std::string foreman_number);

	int login(std::string number, std::string password, std::string &name,
		std::string &last_name, std::string &middle_name, std::string &user_type, std::queue<std::string> &clients_queue);

	std::queue<material> get_materials();

	void store_purchase(std::string foreman_num, std::string client_num, std::queue<purchase> purchases_queue);
};

#endif // DB_CONNECTOR_H

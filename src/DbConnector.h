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

enum status {
	STATUS_FALSE,
	STATUS_TRUE,
	STATUS_NULL
};

struct material {
	std::string title;
	std::string unions;
	double price;
};

struct selected_material {
	std::string title;
	double quantity;
	double cost;
};

struct purchase_to_store {
	std::string client_num;
	std::string foreman_num;
	std::queue<selected_material> materials;
	double total_cost;
};

struct purchase_to_send {
	int id;
	std::queue<selected_material> materials;
	double total_cost;
	status st;
};

class DbConnector {
private:
	MYSQL *_conn_ptr;

	int _get_new_purchase_localid(std::string client_num, std::string foreman_num);

	/*
	* check_for_error() - checks res for error.
	* If there is some error it logs message and throws an exception.
	*/
	void _check_for_error(int res);

public:
	DbConnector();
	~DbConnector();

	std::string register_new(std::string name, std::string last_name,
				std::string middle_name, std::string number,
				std::string password, std::string type,
				std::string foreman_number);

	int login(std::string number, std::string password, std::string &name,
		std::string &last_name, std::string &middle_name, std::string &user_type,
		std::queue<std::string> &clients_queue, std::string &foreman_number);

	std::queue<material> get_materials();

	void store_purchase(purchase_to_store purchase);

	std::queue<purchase_to_send> get_purchases(std::string client_num, std::string foreman_name);
};

#endif // DB_CONNECTOR_H

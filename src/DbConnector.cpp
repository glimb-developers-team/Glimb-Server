/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 15.03.19
*/

#define HOST		"localhost"
#define USER_NAME	"GlimbAdmin"
#define PASSWORD	"secret"
#define DB_NAME		"GlimbRu"

#define QUERY_SIZE	1024

#include "DbConnector.h"
#include "LogPrinter.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <exception>
#include <tuple>

/*
* This file defines class DbConnector, which is described in DbConnector.h.
* See all methods documentation in the header file.
*/

DbConnector::DbConnector()
{
	char query[QUERY_SIZE];

	_conn_ptr = mysql_init(NULL);
	if (_conn_ptr == NULL) {
		LogPrinter::print(mysql_error(_conn_ptr));
		throw std::runtime_error( "MySQL initialization failed");
	}

	_conn_ptr = mysql_real_connect(_conn_ptr, HOST, USER_NAME, PASSWORD, DB_NAME, 0, NULL, 0);
	if (_conn_ptr == NULL) {
		LogPrinter::print(mysql_error(_conn_ptr));
		throw std::runtime_error( "MySQL connection failed");
	}

	snprintf(query, QUERY_SIZE, "SET NAMES \'CP1251\'");
	mysql_query(_conn_ptr, query);

	LogPrinter::print("Successfully connected to the MySQL server");
}

DbConnector::~DbConnector()
{
	mysql_close(_conn_ptr);
	LogPrinter::print("Disconnected from the MySQL server");
}

std::string DbConnector::register_new(std::string name, std::string last_name,
				std::string middle_name, std::string number,
				std::string password, std::string type,
				std::string foreman_number)
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;

	/* Setting query */
	snprintf(query, QUERY_SIZE, "INSERT INTO user(Name, LastName, MiddleName,\
PhoneNumber, Password, Type, ForemanPhoneNumber) VALUES(\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")",
name.c_str(), last_name.c_str(), middle_name.c_str(), number.c_str(),
password.c_str(), type.c_str(), foreman_number.c_str());

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	_check_for_error(res);

	return "OK";
}

int DbConnector::login(std::string number, std::string password, std::string &name,
		std::string &last_name, std::string &middle_name, std::string &user_type,
		std::queue<std::string> &clients_queue)
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	/* Setting query */
	snprintf(query, QUERY_SIZE,
		"SELECT Name, LastName, MiddleName, Type FROM user WHERE PhoneNumber = \"%s\" AND Password = \"%s\";",
		number.c_str(), password.c_str());

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	_check_for_error(res);

	/* Return result */
	mysql_res = mysql_use_result(_conn_ptr);

	sqlrow = mysql_fetch_row(mysql_res);
	if (sqlrow == NULL) {
		return -1;
	}

	name = sqlrow[0];
	last_name = sqlrow[1];
	middle_name = sqlrow[2];
	user_type = sqlrow[3];

	mysql_free_result(mysql_res);

	if (user_type == "foreman") {
		snprintf(query, QUERY_SIZE, "SELECT PhoneNumber FROM user WHERE ForemanPhoneNumber = \"%s\";", number.c_str());

		res = mysql_query(_conn_ptr, query);
		_check_for_error(res);

		mysql_res = mysql_use_result(_conn_ptr);
		while ((sqlrow = mysql_fetch_row(mysql_res)) != NULL) {
			clients_queue.push(sqlrow[0]);
		}
	}

	mysql_free_result(mysql_res);
	return 0;
}

std::queue<material> DbConnector::get_materials()
{
	/* Initialization */
	std::queue<material> materials_queue;
	material tmp;
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	/* Setting query */
	snprintf(query, QUERY_SIZE,
		"SELECT * FROM material;");

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	_check_for_error(res);

	/* Return result */
	mysql_res = mysql_use_result(_conn_ptr);

	sqlrow = mysql_fetch_row(mysql_res);
	while (sqlrow != NULL) {
		tmp.title = sqlrow[0];
		tmp.unions = sqlrow[1];
		tmp.price = atof(sqlrow[2]);

		materials_queue.push(tmp);
		sqlrow = mysql_fetch_row(mysql_res);
	}

	mysql_free_result(mysql_res);
	return materials_queue;
}

void DbConnector::store_purchase(purchase_to_store purchase)
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int purchase_id;
	int local_id;
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	local_id = _get_new_purchase_localid();

	snprintf(query, QUERY_SIZE,
		"INSERT INTO purchase(ClientNum, ForemanNum, TotalCost, LocalId) VALUES(\"%s\", \"%s\", %lf, %d);",
		purchase.client_num.c_str(), purchase.foreman_num.c_str(), purchase.total_cost, local_id);

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	_check_for_error(res);

	mysql_res = mysql_use_result(_conn_ptr);
	mysql_free_result(mysql_res);

	snprintf(query, QUERY_SIZE, "SELECT LAST_INSERT_ID();");

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	_check_for_error(res);

	mysql_res = mysql_use_result(_conn_ptr);
	sqlrow = mysql_fetch_row(mysql_res);
	purchase_id = atoi(sqlrow[0]);
	mysql_free_result(mysql_res);

	/* Pushing data to the storage */
	while (!purchase.materials.empty()) {
		selected_material mat;
		mat = purchase.materials.front();
		snprintf(query, QUERY_SIZE,
			"INSERT INTO purchase_material(PurchaseId, MaterialTitle, Quantity, Cost) VALUES(%d, \"%s\", %lf, %lf);",
			purchase_id, mat.title.c_str(), mat.quantity, mat.cost);

		/* Sending query */
		res = mysql_query(_conn_ptr, query);
		_check_for_error(res);

		purchase.materials.pop();
	}
}

std::queue<purchase_to_send> DbConnector::get_purchases(std::string client_num, std::string foreman_num)
{
	/* Initialization */
	std::queue<purchase_to_send> purchases_queue;
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;
	std::queue<std::tuple<int, int, double>> purdata_queue;

	/* Setting query to get Ids */
	snprintf(query, QUERY_SIZE,
		"SELECT Id, LocalId, TotalCost FROM purchase WHERE ClienNum = \"%s\" AND ForemanNum = \"%s\";",
		client_num.c_str(), foreman_num.c_str());

	/* Sending query */
	res = mysql_query(_conn_ptr, query);
	_check_for_error(res);

	/* Pushing all ids to the queue */
	mysql_res = mysql_use_result(_conn_ptr);
	sqlrow = mysql_fetch_row(mysql_res);
	while (sqlrow != NULL) {
		purdata_queue.push(std::make_tuple(atoi(sqlrow[0]), atoi(sqlrow[1]), atof(sqlrow[2])));
		sqlrow = mysql_fetch_row(mysql_res);
	}
	mysql_free_result(mysql_res);

	/* Filling result queue */
	while (!purdata_queue.empty()) {
		/* Initialization */
		std::tuple<int, int, double> cur_data = purdata_queue.front();
		std::queue<selected_material> cur_materials_queue;
		purchase_to_send cur_purchase;

		/* Setting query to get current purchase materials */
		snprintf(query, QUERY_SIZE,
			"SELECT MaterialTitle, Quantity, Cost WHERE PurchaseId = %d;",
			std::get<1>(cur_data));

		/* Sending query */
		res = mysql_query(_conn_ptr, query);
		_check_for_error(res);

		/* Getting all materials */
		mysql_res = mysql_use_result(_conn_ptr);
		sqlrow = mysql_fetch_row(mysql_res);
		while (sqlrow != NULL) {
			selected_material cur_material;

			cur_material.title = sqlrow[0];
			cur_material.quantity = atof(sqlrow[1]);
			cur_material.cost = atof(sqlrow[2]);

			cur_materials_queue.push(cur_material);

			sqlrow = mysql_fetch_row(mysql_res);
		}
		mysql_free_result(mysql_res);

		cur_purchase.id = std::get<0>(cur_data);
		cur_purchase.materials = cur_materials_queue;
		cur_purchase.total_cost = std::get<2>(cur_data);

		purchases_queue.push(cur_purchase);

		purdata_queue.pop();
	}
}

int DbConnector::_get_new_purchase_localid()
{
	/* Initialization */
	char query[QUERY_SIZE];
	char error[QUERY_SIZE];
	int local_id;
	int res;
	MYSQL_RES *mysql_res;
	MYSQL_ROW sqlrow;

	snprintf(query, QUERY_SIZE, "SELECT MAX(LocalId) FROM purchase;");

	res = mysql_query(_conn_ptr, query);
	if (res != 0) {	// Select failed
		snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw std::runtime_error("MySQL select failed");
	}

	mysql_res = mysql_use_result(_conn_ptr);
	sqlrow = mysql_fetch_row(mysql_res);
	local_id = atoi(sqlrow[0]) + 1;

	mysql_free_result(mysql_res);
	return local_id;
}

void DbConnector::_check_for_error(int res)
{
	char error[QUERY_SIZE];

	if (res != 0) {	// Mysql error
		snprintf(error, QUERY_SIZE, "MySQL error: %s", mysql_error(_conn_ptr));
		LogPrinter::print(error);
		throw std::runtime_error("MySQL query failed");
	}
}

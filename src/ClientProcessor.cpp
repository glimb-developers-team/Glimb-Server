/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 13.03.19
*/

#include "ClientProcessor.h"
#include "requests.h"
#include "LogPrinter.h"
#include <thread>
#include <chrono>
#include <cstring>
#include <string>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <exception>
#include "rapidjson/prettywriter.h"

/*
* This file defines methods of class ClientProcessor, which is described in ClientProcessor.h.
* See all methods documentation in the header file.
*/

#define BUF_SIZE 1024
#define MATERIAL_SYM_LENGTH 256
#define PURCHASE_SYM_LENGTH 256
#define TIMEOUT 100

void send_answer(int client_sockfd, rapidjson::Document &document);
void send_error(int client_sockfd, std::string error);
void send_ok(int client_sockfd);
void check_field(const rapidjson::Value &value, std::string field);
void add_strfield(rapidjson::Value &value, const std::string field, const std::string str, rapidjson::Document::AllocatorType &alloc);

ClientProcessor::ClientProcessor() : _db_connector(DbConnector()), _clients_counter(0)
{
	for (int i = 0; i < MAX_CLIENTS; i++) {
		_clients[i] = -1;
	}
}

ClientProcessor::~ClientProcessor()
{
	for (int i : _clients) {
		if (i != -1) {
			close(i);
		}
	}
	LogPrinter::print("All clients sockets were closed");
}

void ClientProcessor::new_client(int client_sockfd)
{
	int i;

	if (_clients_counter < MAX_CLIENTS) {
		for (i = 0; i < MAX_CLIENTS; i++) {
			if (_clients[i] == -1) {
				_clients[i] = client_sockfd;
				_clients_counter++;
				break;
			}
		}
		std::thread client_thread(&ClientProcessor::_processing_client, this, i);
		client_thread.detach();
	}
	else {
		send_error(client_sockfd, "Too much connections, try later");
	}
}

void ClientProcessor::_processing_client(int client_num)
{
	/* Initialization */
	char log_message[80];
	char buffer[BUF_SIZE];
	char log_buffer[BUF_SIZE];
	rapidjson::Document document;
	std::string request;
	int res;

	snprintf(log_message, 80,
		"New thread started for client on socket %d", _clients[client_num]);
	LogPrinter::print(log_message);

	/* Start processing */
	res = recv(_clients[client_num], buffer, BUF_SIZE, 0);
	if (res == 0) {
		LogPrinter::print("Received 0 symbols");
	}
	while (res > 0) {
		try {
			snprintf(log_buffer, BUF_SIZE, "Received %d symbols: %s", res, buffer);
			LogPrinter::print(log_buffer);

			/* Some cheсks */
			if (document.Parse(buffer).HasParseError()) {
				throw std::runtime_error("Request syntax error");
			}
			if (!document.HasMember("request")) {
				throw std::runtime_error("No request field");
			}
			if (!document["request"].IsString()) {
				throw std::runtime_error("\"request\" field isn't a string type");
			}
			if (!document.HasMember("info")) {
				throw std::runtime_error("No info field");
			}
			if (!document["info"].IsObject()) {
				throw std::runtime_error("Info isn't an object type");
			}

			/* Switсhing requests */
			request = document["request"].GetString();
			if (request == REQUEST_REGISTRATION) {
				_register(_clients[client_num], document["info"]);
			}
			else if (request == REQUEST_LOGIN) {
				_login(_clients[client_num], document["info"]);
			}
			else if (request == REQUEST_GET_MATERIALS) {
				_send_materials(_clients[client_num]);
			}
			else if (request == REQUEST_SEND_PURCHASE) {
				_recv_purchase(_clients[client_num], document["info"]);
			}
			else if (request == REQUEST_GET_PURCHASES) {
				_send_purchases(_clients[client_num], document["info"]);
			}
		}
		catch (const std::exception &error) {
			send_error(_clients[client_num], error.what());
		}

		res = recv(_clients[client_num], buffer, BUF_SIZE, 0);
	}
	/* Closing connection */
	snprintf(log_buffer, BUF_SIZE, "Connection closed with client on socket %d", _clients[client_num]);
	LogPrinter::print(log_buffer);
	close(_clients[client_num]);
	_clients[client_num] = -1;
	_clients_counter--;
}

void ClientProcessor::_register(int client_sockfd, rapidjson::Value &info)
{
	/* Initialization */
	std::string db_answer;
	char buffer[BUF_SIZE];
	rapidjson::Document new_document;
	rapidjson::Document::AllocatorType& alloc = new_document.GetAllocator();
	rapidjson::Value value;

	check_field(info, "name");
	check_field(info, "last_name");
	check_field(info, "middle_name");
	check_field(info, "number");
	check_field(info, "password");
	check_field(info, "type");
	check_field(info, "foreman_number");

	db_answer = _db_connector.register_new(info["name"].GetString(), info["last_name"].GetString(),
			info["middle_name"].GetString(), info["number"].GetString(),
			info["password"].GetString(), info["type"].GetString(),
			info["foreman_number"].GetString());

	if (db_answer == "OK") {
		send_ok(client_sockfd);
	}
	else {
		if (db_answer.find("ERROR") && db_answer.find("Duplicate") && db_answer.find("PRIMARY")) {
			throw std::runtime_error("This user already exists");
		}
		else {
			throw std::runtime_error("Unknown database error");
		}
	}
}

void ClientProcessor::_login(int client_sockfd, rapidjson::Value &info)
{
	/* Initialization */
	int db_answer;
	std::string name;
	std::string last_name;
	std::string middle_name;
	std::string user_type;
	std::queue<std::string> clients_numbers;
	rapidjson::Document document;
	rapidjson::Value value;
	rapidjson::Value tmp;
	rapidjson::Value numbers_mas;
	rapidjson::Document::AllocatorType& alloc = document.GetAllocator();

	check_field(info, "number");
	check_field(info, "password");

	db_answer = _db_connector.login(info["number"].GetString(),
					info["password"].GetString(),
					name, last_name, middle_name, user_type, clients_numbers);

	if (db_answer != 0) {	// Error
		throw std::runtime_error("Wrong number or password");
	}
	else {			// Someone was found
		/* Setting json */
		document.SetObject();
		document.AddMember("type", "ok", alloc);
		value.SetObject();

		add_strfield(value, "name", name, alloc);

		add_strfield(value, "last_name", last_name, alloc);

		add_strfield(value, "middle_name", middle_name, alloc);

		add_strfield(value, "user_type", user_type, alloc);

		if (user_type == "foreman") {
			numbers_mas.SetArray();
			while (!clients_numbers.empty()) {
				tmp.SetString(clients_numbers.front().c_str(), alloc);
				numbers_mas.PushBack(tmp, alloc);
				clients_numbers.pop();
			}
			value.AddMember("clients_numbers", numbers_mas, alloc);
		}

		document.AddMember("info", value, alloc);

		/* Sending json */
		send_answer(client_sockfd, document);
	}
}

void ClientProcessor::_send_materials(int client_sockfd)
{
	/* Initialization */
	std::queue<material> materials_queue = _db_connector.get_materials();
	rapidjson::Document document;
	rapidjson::Value info;
	rapidjson::Value materials;
	rapidjson::Value mat_obj;
	rapidjson::Value str;
	rapidjson::Document::AllocatorType& alloc = document.GetAllocator();
	int i;

	/* Sending data about all materials */
	while (materials_queue.empty() != true) {
		/* Setting json */
		document.SetObject();
		document.AddMember("type", "ok", alloc);
		info.SetObject();
		materials.SetArray();
		i = 0;

		while ((BUF_SIZE - MATERIAL_SYM_LENGTH*(++i) >= 128) &&	// Until lost 128 bytes of free space
		(materials_queue.empty() != true)) {
			mat_obj.SetObject();
			material cur_material =  materials_queue.front();

			add_strfield(mat_obj, "title", cur_material.title, alloc);

			add_strfield(mat_obj, "unions", cur_material.unions, alloc);

			str = rapidjson::Value(materials_queue.front().price);
			mat_obj.AddMember("price", str, alloc);

			materials.PushBack(mat_obj, alloc);
			materials_queue.pop();
		}
		info.AddMember("materials", materials, alloc);
		document.AddMember("info", info, alloc);

		/* Sending json */
		send_answer(client_sockfd, document);
	}

	/* Setting end message */
	document.SetObject();
	document.AddMember("type","ok", alloc);
	info.SetObject();
	info.AddMember("description", "end", alloc);
	document.AddMember("info", info, alloc);

	/* Sending end message */
	send_answer(client_sockfd, document);
}

void ClientProcessor::_recv_purchase(int client_sockfd, rapidjson::Value &info)
{
	LogPrinter::print("Starting _recv_purchase");
	/* Initialization */
	static purchase_to_store purchase;
	rapidjson::Document document;
	rapidjson::Value purchase_value;
	rapidjson::Value output_info;
	rapidjson::Document::AllocatorType& alloc = document.GetAllocator();

	document.SetObject();
	document.AddMember("type", "ok", alloc);
	output_info.SetObject();
	document.AddMember("info", output_info, alloc);

	/* If it's the end of translation */
	if (info.HasMember("description")) {
		if (strcmp(info["description"].GetString(), "end") == 0) {
			/* Storing data in db */
			LogPrinter::print("Pushing to db");
			_db_connector.store_purchase(purchase);
			purchase = purchase_to_store();
			LogPrinter::print("Sending answer");
			send_answer(client_sockfd, document);
			return;
		}
	}

	check_field(info, "foreman_num");
	purchase.foreman_num = info["foreman_num"].GetString();

	check_field(info, "client_num");
	purchase.client_num = info["client_num"].GetString();

	check_field(info, "total_cost");
	purchase.total_cost = info["total_cost"].GetDouble();

	check_field(info, "purchase");
	purchase_value = info["purchase"];

	/* Storing data */
	LogPrinter::print("Starting storing data");
	for (rapidjson::Value::ConstValueIterator itr = purchase_value.Begin(); itr != purchase_value.End(); itr++) {
		selected_material cur_material;
		rapidjson::Value::ConstMemberIterator currentElement = itr->FindMember("title");
		cur_material.title = currentElement->value.GetString();

		currentElement = itr->FindMember("quantity");
		cur_material.quantity = currentElement->value.GetDouble();

		currentElement = itr->FindMember("cost");
		cur_material.cost = currentElement->value.GetDouble();

		purchase.materials.push(cur_material);
	}

	return;
}

void ClientProcessor::_send_purchases(int client_sockfd, rapidjson::Value &info)
{
	/* Initialization */
	std::queue<purchase_to_send> purchases_queue;
	rapidjson::Value client_num_value;
	rapidjson::Value foreman_num_value;
	rapidjson::Document answer;
	rapidjson::Document::AllocatorType& alloc = answer.GetAllocator();
	rapidjson::Value answer_info;
	char buffer[BUF_SIZE];

	check_field(info, "client_num");
	check_field(info, "foreman_num");

	client_num_value = info["client_num"];
	foreman_num_value = info["foreman_num"];

	purchases_queue = _db_connector.get_purchases(client_num_value.GetString(),
							foreman_num_value.GetString());

	while (!purchases_queue.empty()) {
		purchase_to_send &cur_purchase = purchases_queue.front();
		std::string str_status;

		/* Sending purchase initial message */
		answer.SetObject();
		answer.AddMember("type", "ok", alloc);
		answer_info.SetObject();
		answer_info.AddMember("purchase_id",
					rapidjson::Value(cur_purchase.id), alloc);
		answer_info.AddMember("total_cost",
					rapidjson::Value(cur_purchase.total_cost), alloc);
		switch (cur_purchase.st) {
		case STATUS_TRUE:
			str_status = "true";
			break;
		case STATUS_FALSE:
			str_status = "false";
			break;
		default:
			str_status = "null";
			break;
		}
		add_strfield(answer_info, "status", str_status, alloc);
		answer_info.AddMember("method", "start", alloc);
		answer.AddMember("info", answer_info, alloc);
		send_answer(client_sockfd, answer);

		/* Sending materials in the current purchase */
		while (!cur_purchase.materials.empty()) {
			rapidjson::Value materials;
			int i = 0;

			answer.SetObject();
			answer.AddMember("type", "ok", alloc);
			answer_info.SetObject();
			materials.SetArray();

			/* Materials list formation */
			while (BUF_SIZE - PURCHASE_SYM_LENGTH * (++i) >= 128 &&
			!cur_purchase.materials.empty()) {
				rapidjson::Value cur_material_obj;
				selected_material &cur_material = cur_purchase.materials.front();

				cur_material_obj.SetObject();

				add_strfield(cur_material_obj, "title", cur_material.title, alloc);
				cur_material_obj.AddMember("quantity",
							rapidjson::Value(cur_material.quantity),
							alloc);
				cur_material_obj.AddMember("cost",
							rapidjson::Value(cur_material.cost),
							alloc);

				materials.PushBack(cur_material_obj, alloc);

				cur_purchase.materials.pop();
			}

			answer_info.AddMember("materials", materials, alloc);
			answer.AddMember("info", answer_info, alloc);
			send_answer(client_sockfd, answer);
		}

		/* Sending purchase ending message */
		answer.SetObject();
		answer.AddMember("type", "ok", alloc);
		answer_info.SetObject();
		answer_info.AddMember("purchase_id",
					rapidjson::Value(cur_purchase.id), alloc);
		answer_info.AddMember("method", "end", alloc);
		answer.AddMember("info", answer_info, alloc);
		send_answer(client_sockfd, answer);

		purchases_queue.pop();
	}

	/* Sending translation ending message */
	answer.SetObject();
	answer.AddMember("type", "ok", alloc);
	answer_info.SetObject();
	answer_info.AddMember("method", "end of translation", alloc);
	answer.AddMember("info", answer_info, alloc);
	send_answer(client_sockfd, answer);
}

void check_field(const rapidjson::Value &info, std::string field)
{
	/* Initialization */
	char exception[80];

	if (!info.HasMember(field.c_str())) {
		snprintf(exception, 80, "No %s field", field.c_str());
		throw std::runtime_error(exception);
	}
}

void send_answer(int client_sockfd, rapidjson::Document &document)
{
	/* Initialization */
	static std::chrono::milliseconds last_time;
	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds diff;
	rapidjson::StringBuffer string_buf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buf);
	char buffer[BUF_SIZE];
	char log_buffer[BUF_SIZE];

	diff = now - last_time;
	if(diff.count() < TIMEOUT) {
		LogPrinter::print("Timeout for " + std::to_string(diff.count()) + " milliseconds");
		usleep(diff.count() * 1000);
	}

	/* Conversion to the char* */
	document.Accept(writer);
	snprintf(buffer, BUF_SIZE, "%s", string_buf.GetString());

	/* Sending answer */
	snprintf(log_buffer, BUF_SIZE, "Sending to the client on the socket %d message: %s",
		client_sockfd, buffer);
	LogPrinter::print(log_buffer);
	send(client_sockfd, buffer, BUF_SIZE, 0);
	last_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

void send_error(int client_sockfd, std::string error)
{
	/* Initialization */
	rapidjson::Document document;
	rapidjson::Value value;
	rapidjson::Value error_value;
	rapidjson::Document::AllocatorType& alloc = document.GetAllocator();

	/* Setting json answer */
	document.SetObject();
	document.AddMember("type", "error", alloc);
	value.SetObject();
	add_strfield(value, "description", error, alloc);
	document.AddMember("info", value, alloc);

	/* Sending answer */
	send_answer(client_sockfd, document);
}

void send_ok(int client_sockfd)
{
	/* Initialization */
	rapidjson::Document document;
	rapidjson::Value value;
	rapidjson::Document::AllocatorType& alloc = document.GetAllocator();

	/* Setting json answer */
	document.SetObject();
	document.AddMember("type", "ok", alloc);
	value.SetObject();
	document.AddMember("info", value, alloc);

	/* Sending answer */
	send_answer(client_sockfd, document);
}

void add_strfield(rapidjson::Value &value, const std::string field, const std::string str, rapidjson::Document::AllocatorType &alloc)
{
	rapidjson::Value str_value;
	rapidjson::Value field_value;

	str_value.SetString(str.c_str(), alloc);
	field_value.SetString(field.c_str(), alloc);
	value.AddMember(field_value, str_value, alloc);
}

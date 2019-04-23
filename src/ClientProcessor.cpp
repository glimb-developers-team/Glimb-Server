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
#include <cstring>
#include <string>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "rapidjson/prettywriter.h"

/*
* This file defines methods of class ClientProcessor, which is described in ClientProcessor.h.
* See all methods documentation in the header file.
*/

#define BUF_SIZE 512
#define MATERIAL_SYM_LENGTH 128

void send_answer(int client_sockfd, rapidjson::Document &document);
void send_error(int client_sockfd, std::string error);
void send_ok(int client_sockfd);
void check_field(rapidjson::Value &value, std::string field);

ClientProcessor::ClientProcessor() : _db_connector(DbConnector()), _clients_counter(0)
{
	for (int i : _clients) {
		i = -1;
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
	if (_clients_counter < MAX_CLIENTS) {
		_clients[_clients_counter++] = client_sockfd;
		std::thread client_thread(&ClientProcessor::_processing_client, this, _clients_counter - 1);
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
				throw "Request syntax error";
			}
			if (!document.HasMember("request")) {
				throw "No request field";
			}
			if (!document["request"].IsString()) {
				throw "\"request\" field isn't a string type";
			}
			if (!document.HasMember("info")) {
				throw "No info field";
			}
			if (!document["info"].IsObject()) {
				throw "Info isn't an object type";
			}

			/* Swithing requests */
			request = document["request"].GetString();
			if (request == REQUEST_REGISTRATION) {
				_register(_clients[client_num], document["info"]);
			}
			else if (request == REQUEST_LOGIN) {
				_login(_clients[client_num], document["info"]);
			}
			else if (request == REQUEST_GET_MATERIALS) {
				_get_materials(_clients[client_num]);
			}
		}
		catch (const char *error) {
			send_error(_clients[client_num], error);
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

	LogPrinter::print("Starting registrarion");

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
			send_error(client_sockfd, "This user already exists");
		}
		else {
			send_error(client_sockfd, "Unknown database error");
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
	rapidjson::Document document;
	rapidjson::Value value;
	rapidjson::Value tmp;
	rapidjson::Document::AllocatorType& alloc = document.GetAllocator();

	LogPrinter::print("Starting login");

	check_field(info, "number");
	check_field(info, "password");

	db_answer = _db_connector.login(info["number"].GetString(),
					info["password"].GetString(),
					name, last_name, middle_name, user_type);

	if (db_answer != 0) {	// Error
		send_error(client_sockfd, "Wrong number or password");
	}
	else {			// Someone was found
		/* Setting json */
		document.SetObject();
		document.AddMember("type", "ok", alloc);
		value.SetObject();
		tmp.SetString(rapidjson::StringRef(name.c_str()));
		value.AddMember("name", tmp, alloc);
		tmp.SetString(rapidjson::StringRef(last_name.c_str()));
		value.AddMember("last_name", tmp, alloc);
		tmp.SetString(rapidjson::StringRef(middle_name.c_str()));
		value.AddMember("middle_name", tmp, alloc);
		tmp.SetString(rapidjson::StringRef(user_type.c_str()));
		value.AddMember("user_type", tmp, alloc);
		document.AddMember("info", value, alloc);

		/* Sending json */
		send_answer(client_sockfd, document);
	}
}

void ClientProcessor::_get_materials(int client_sockfd)
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
			str.SetString(materials_queue.front().title.c_str(), alloc);
			mat_obj.AddMember("title", str, alloc);
			str.SetString(materials_queue.front().unions.c_str(), alloc);
			mat_obj.AddMember("unions", str, alloc);
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

	document.SetObject();
	document.AddMember("type","ok", alloc);
	info.SetObject();
	info.AddMember("description", "end", alloc);
	document.AddMember("info", info, alloc);

	send_answer(client_sockfd, document);
}

void check_field(rapidjson::Value &info, std::string field)
{
	/* Initialization */
	char exception[80];

	if (!info.HasMember(field.c_str())) {
		snprintf(exception, 80, "No \"%s\" field", field.c_str());
		throw (const char*)exception;
	}
	if (!info[field.c_str()].IsString()) {
		snprintf(exception, 80, "\"%s\" field ins't a string type", field.c_str());
		throw (const char*)exception;
	}
}

void send_answer(int client_sockfd, rapidjson::Document &document)
{
	/* Initialization */
	rapidjson::StringBuffer string_buf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buf);
	char buffer[BUF_SIZE];
	char log_buffer[BUF_SIZE];

	/* Conversion to the char* */
	document.Accept(writer);
	snprintf(buffer, BUF_SIZE, "%s", string_buf.GetString());

	/* Sending answer */
	snprintf(log_buffer, BUF_SIZE, "Sending to the client on the socket %d message: %s",
		client_sockfd, buffer);
	LogPrinter::print(log_buffer);
	send(client_sockfd, buffer, BUF_SIZE, 0);
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
	error_value.SetString(rapidjson::StringRef(error.c_str()));
	value.AddMember("description", error_value, alloc);
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

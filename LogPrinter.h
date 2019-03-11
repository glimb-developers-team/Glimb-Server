/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 11.03.19
*/

#ifndef LOG_PRINTER_H
#define LOG_PRINTER_H

/*
* This file describes class LogPrinter for printer log messages.
* It appends data at the end of the file, which name was passed as a parameter in counstructor.
*/

#include <fstream>
#include <string>

class LogPrinter {
private:
	std::ofstream log_file;

public:
	/*
	* LogPrinter() - a name of a log file should be passed as a parameter.
	* Data will be written at the end of file.
	*/
	LogPrinter(std::string file_name);

	~LogPrinter();

	/*
	* print() - prints current time and the log message at the end of the log file.
	*/
	void print(std::string message);

	/*
	* error() - prints current time, the ERROR label
	* and error message at the end of the log file.
	*/
	void error(std::string error_message);
};

#endif // LOG_PRINTER_H

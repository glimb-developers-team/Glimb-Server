/*
* author: Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 08.06.19
*/

#include "DbArg.h"
#include "DbResult.h"
#include "mysql.h"

namespace DbConnector::DbQueryHandler {
	/*
	* DbQueryHandlerBase - base abstract class for db query handlers.
	* Every handler should be extended from this class and should
	* override handle() method.
	*/
	class DbQueryHandlerBase {
	private:
		static MYSQL *_conn_ptr;
		static unsigned int _instance_counter;

	public:
		DbQueryHandlerBase();
		~DbQueryHandlerBase();

		/*
		* handle() - main method of handler. Should be overrided
		* in every child class.
		* As an real argument should be expected struct extended
		* from DbArg::DbArg, where all needed parameters can be added.
		* All DbArg child classes should be places in DbArgs directory.
		* All the above is similarly usable for method result.
		*/
		virtual DbResult::DbResult handle(DbArg::DbArg arg) = 0;

	protected:
		/*
		* Here are some wrappers of MySQL API function.
		* They are needed to limit access to the _conn_ptr variable.
		* It is enough for getting all wanted data from Db.
		*/
		int _mysql_query(const char *query);
		MYSQL_RES* _mysql_use_result();
		MYSQL_ROW _mysql_fetch_row(MYSQL_RES *mysql_res);
		void _mysql_free_result(MYSQL_RES *mysql_res);

		/*
		* _check_for_error() - checks res from MySQL API for error
		* and throws exception of type std::runtime_error if there was
		* an error.
		*/
		void _check_for_error(int res);
	};
}

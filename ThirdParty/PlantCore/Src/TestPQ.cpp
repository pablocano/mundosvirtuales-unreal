#include <iostream>
#include <sstream>
#include <pqxx/pqxx>

/// Query employees from database.  Return result.
std::string query()
{
	try
	{
		pqxx::connection c{ "dbname=dvdrental user=postgres" };
		pqxx::work txn{ c };

		pqxx::result r = txn.exec("SELECT * FROM actor where first_name='John'");
		std::stringstream ss;
		for (auto row : r)
		{
			for (auto field : row) ss << field.c_str() << " ";
			ss << std::endl;
		}

		// Not really needed, since we made no changes, but good habit to be
		// explicit about when the transaction is done.
		txn.commit();

		// Connection object goes out of scope here.  It closes automatically.
		return ss.str();
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << "Error: " << e.what() << std::endl;
		return ss.str();
	}
	catch (const pqxx::sql_error &e)
	{
		std::stringstream ss;
		ss << "SQL error: " << e.what() << std::endl;
		ss << "Query was: " << e.query() << std::endl;
		return ss.str();
	}
}

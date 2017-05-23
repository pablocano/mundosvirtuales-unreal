#include "TestPQ.h"

#include <iostream>
#include <sstream>

#include "soci/soci.h"
#include "soci/postgresql/soci-postgresql.h"

/// Query employees from database.  Return result.
void query(char* str, int len)
{
	try
	{
		soci::session sql( "postgresql", "dbname=dvdrental user=postgres" );
		
		soci::rowset<> rows(sql.prepare << "SELECT * FROM actor where last_name like 'Z%'");
		std::stringstream ss;
		for (auto it = rows.begin(); it != rows.end(); ++it)
		{
			ss << it->get<std::string>(1) << " " << it->get<std::string>(2) << std::endl;
		}

		// Connection object goes out of scope here.  It closes automatically.
		ss.str().copy(str, len, 0);
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << "Error: " << e.what() << std::endl;
		ss.str().copy(str, len, 0);
	}
}

// Fill out your copyright notice in the Description page of Project Settings

#include "Plant.h"
#include "TestPQ.h"

/// Query employees from database.  Return result.
void query(char* buffer, int len)
{
	try
	{
		pqxx::connection c{ "dbname=dvdrental user=postgres" };
		pqxx::work txn{ c };

		pqxx::result r = txn.exec("SELECT * FROM actor where first_name='John'");
		std::stringstream ss("");
		for (auto row : r)
		{
			for (auto field : row) ss << field.c_str() << " ";
			ss << std::endl;
		}

		// Not really needed, since we made no changes, but good habit to be
		// explicit about when the transaction is done.
		txn.commit();

		// Connection object goes out of scope here.  It closes automatically.
		ss.str().copy(buffer, len, 0);
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << "Error: " << e.what() << std::endl;
		ss.str().copy(buffer, len, 0);
	}
}

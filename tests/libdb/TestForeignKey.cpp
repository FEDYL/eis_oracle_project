#include "Tests.h"

void testForeignKey(bool isDropOldDB) {
	DB_Oracle::Factory::createInstance();
	PRINT_INFO("\nTEST FOREIGN KEY")
	try {
		PRINT_INFO_BEGIN("Connection") 
		DB::Connection_ptr connection = DB::Factory::getInstance()->createNoLogConnection(HOST, USER, PASS);
                PRINT_INFO_END

		if (isDropOldDB) {
			PRINT_INFO_BEGIN("Drop old base")		
			connection->dropDataBase("libdbtest");
			PRINT_INFO_END	
		}

		PRINT_INFO_BEGIN("Create base")
		connection->createDataBase("libdbtest");
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Use Base")
		connection->useDataBase("libdbtest");
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Create table A")
		DB::Table_ptr tableA = DB::Factory::getInstance()->createTable("TableA");
		tableA->addFieldDef( new DB::LongFieldDef("a") );
		tableA->addFieldDef( new DB::FloatFieldDef("b") );
		tableA->addFieldDef( new DB::StringFieldDef("c") );
		tableA->addFieldDef( new DB::TimeFieldDef("d") );
		tableA->addPrimaryKey().addFieldDef("a");
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Add table A to base")
		connection->createObject(tableA.get());
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Insert rows to table A")
		DB::Query query;
		query = "INSERT INTO TableA (a, b, c, d) VALUES (1, 2.2, \"One\", \"20:23:14\")";
		connection->execSQL(query);
		query.clear();
		query = "INSERT INTO TableA (a, b, c, d) VALUES (2, -3.4, \"Two\", \"21:00\")";
		connection->execSQL(query);
		query.clear();
		query = "INSERT INTO TableA (a, b, c, d) VALUES (3, 0.0, \"Three\", \"23:59:59\")";
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END
		
		PRINT_INFO_BEGIN("Create table B")
		DB::Table_ptr tableB = DB::Factory::getInstance()->createTable("TableB");
		tableB->addFieldDef( new DB::LongFieldDef("x") );
		tableB->addFieldDef( new DB::DateFieldDef("y") );
		tableB->addFieldDef( new DB::LongFieldDef("z") );
		tableB->addPrimaryKey().addFieldDef("x");
		tableB->addForeignKey("a", "TableA", DB::ForeignKey::ON_DELETE_SET_NULL).addReference("z", "a"); 
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Add table B to base")
		connection->createObject(tableB.get());
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Insert rows to table B")
		query = "INSERT INTO TableB (x, y, z) VALUES (10, \"1990.01.01\", 2)";
		connection->execSQL(query);
		query.clear();
		query = "INSERT INTO TableB (x, y, z) VALUES (20, \"1990.01.02\", 1)";
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END

		query = "SELECT * FROM TableA";
		DB::AutoResultSet rs( connection->execSQL(query) );
		PRINT_INFO("Table A:")
		for(DB::ResultSet::iterator i = rs->begin(); i != rs->end(); ++i) {
			DB::ResultRow &row = *i;
			std::cout << "(" << row[0]->asLong() << ", " 
					 << row[1]->asFloat() << ", \""
					 << row[2]->asString() << "\","
					 << row[3]->asTime() << ")" << std::endl;
		}

		query = "SELECT * FROM TableB";
		rs = connection->execSQL(query);
		PRINT_INFO("Table B:")
		for(DB::ResultSet::iterator i = rs->begin(); i != rs->end(); ++i) {
			DB::ResultRow &row = *i;
			std::cout << "(" << row[0]->asLong() << ", " 
					 << row[1]->asDate() << ", "
					 << row[2]->asLong() << ")" << std::endl;
		}
		
		PRINT_INFO_BEGIN("Delete row from table A")
		query = "DELETE FROM TableA WHERE TableA.a = 1";
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END
		
		PRINT_INFO_BEGIN("Drop tables")
		connection->dropObject(tableB.get());
		connection->dropObject(tableA.get());
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Drop base")
		connection->dropDataBase("libdbtest");	
		PRINT_INFO_END
		PRINT_INFO("\n")

	} catch( const DB::XDBError &err ) {
		return;
	}
}

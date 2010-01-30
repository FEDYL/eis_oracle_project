#include "Tests.h"
#include "libdb/oracle/field.hpp"
#include <vector>

void testDataBase(bool isDropOldDB) {
	DB_Oracle::Factory::createInstance();
	DB::Query query;
	PRINT_INFO("\nTEST DATA BASE")
	try {
		PRINT_INFO_BEGIN("Connection") 
		DB::Connection_ptr connection = DB::Factory::getInstance()->createNoLogConnection("", USER, PASS);
                PRINT_INFO_END

		if(isDropOldDB)	{
			query = "DROP TABLE Table1";
			connection->execSQL(query);
			query.clear();
		}

		PRINT_INFO_BEGIN("Create table")
		DB::Table_ptr table1 = DB::Factory::getInstance()->createTable("Table1");
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Create LongField field")
		table1->addFieldDef( new DB_Oracle::LongFieldDef("LongField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create ULongField field")
		table1->addFieldDef( new DB_Oracle::ULongFieldDef("ULongField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create ShortField field")
		table1->addFieldDef( new DB_Oracle::ShortFieldDef("ShortField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create UShortField field")
		table1->addFieldDef( new DB_Oracle::UShortFieldDef("UShortField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create LongLongField field")
		table1->addFieldDef( new DB_Oracle::LongLongFieldDef("LongLongField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create ULongLongField field")
		table1->addFieldDef( new DB_Oracle::ULongLongFieldDef("ULongLongField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create FloatField field")
		table1->addFieldDef( new DB_Oracle::FloatFieldDef("FloatField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create DoubleField field")
		table1->addFieldDef( new DB_Oracle::DoubleFieldDef("DoubleField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create CharField field")
		table1->addFieldDef( new DB_Oracle::CharFieldDef("CharField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create BoolField field")
		table1->addFieldDef( new DB_Oracle::BoolFieldDef("BoolField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create StringField field")
		table1->addFieldDef( new DB_Oracle::StringFieldDef("StringField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create BlobField field")
		table1->addFieldDef( new DB_Oracle::BlobFieldDef("BlobField") );
		PRINT_INFO_END 
		PRINT_INFO_BEGIN("Create DateField field")
		table1->addFieldDef( new DB_Oracle::DateFieldDef("DateField") );
		PRINT_INFO_END 
		PRINT_INFO_BEGIN("Create TimeField field")
		table1->addFieldDef( new DB_Oracle::TimeFieldDef("TimeField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create DateTimeField field")
		table1->addFieldDef( new DB_Oracle::DateTimeFieldDef("DateTimeField") );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Create Numeric field")
		table1->addFieldDef( new DB_Oracle::NumericFieldDef("NumericField", 7, 4) );
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Set primary key")
		table1->addPrimaryKey().addFieldDef("LongField");
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Add table to base")
		connection->createObject(table1.get());
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Create insert 1st row query")
		query = "INSERT INTO Table1 (LongField, ULongField, ShortField, UShortField, LongLongField, ULongLongField, FloatField, DoubleField, CharField, BoolField, StringField, BlobField, DateField, TimeField, DateTimeField, NumericField) VALUES (1, 2000, -12, 2332, -1234543, 323434, 3.4, 34532.463, 'e', 1, 'One', '12', to_date('2009-10-20','yyyy-mm-dd'), to_timestamp('8:23:14', 'hh:mi:ss'), to_timestamp('1990-01-01 11:22:33', 'yyyy-mm-dd hh:mi:ss'), 123.12)";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END
		
		PRINT_INFO_BEGIN("Create insert 2nd row query")
		query = "INSERT INTO Table1 (LongField, ULongField, ShortField, UShortField, LongLongField, ULongLongField, FloatField, DoubleField, CharField, BoolField, StringField, BlobField, DateField, TimeField, DateTimeField, NumericField) VALUES (2, 0, -2, 123, 9887, 999826, -1.4, 3431.00043, '0', 0, 'Two', '7f', to_date('1990-01-01','yyyy-mm-dd'), to_timestamp('9:00', 'hh:mi'), to_timestamp('1990-01-01 08:00', 'yyyy-mm-dd hh:mi'), 66.4)";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END
		
		PRINT_INFO_BEGIN("Create insert 3rd row query")
		query = "INSERT INTO Table1 (LongField, ULongField, ShortField, UShortField, LongLongField, ULongLongField, FloatField, DoubleField, CharField, BoolField, StringField, BlobField, DateField, TimeField, DateTimeField, NumericField) VALUES (3, 12, 176, 22, 123, 1628392, 89, -345.0063, '>', 1, 'three', 'a', to_date('1997-12-31','yyyy-mm-dd'), to_timestamp('3:00:01', 'hh:mi:ss'), to_timestamp('1990-1-01 10:00:01', 'yyyy-mm-dd hh:mi:ss'), 0)";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END


		PRINT_INFO_BEGIN("Create select query")
		query="SELECT * FROM Table1";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		DB::AutoResultSet rs(connection->execSQL(query));
		PRINT_INFO_END

		PRINT_INFO("Result of select:")
		unsigned int k = 0;
		for(DB::ResultSet::iterator i = rs->begin(); i != rs->end(); ++k, ++i) {
			std::cout << "row #" << k + 1 << ":" << std::endl;
			DB::ResultRow &row = *i;
			std::cout
				<< "\tLongField = "		<< row[k * 16]->asLong()
				<< "\n\tULongField = "		<< row[k * 16 + 1]->asULong()
				<< "\n\tShortField = "		<< row[k * 16 + 2]->asShort()
				<< "\n\tUShortField = "		<< row[k * 16 + 3]->asUShort()
				<< "\n\tLongLongField = "	<< row[k * 16 + 4]->asLongLong()
				<< "\n\tULongLongField = "	<< row[k * 16 + 5]->asULongLong()
				<< "\n\tFloatField = "		<< row[k * 16 + 6]->asFloat()
				<< "\n\tDoubleField = "		<< row[k * 16 + 7]->asDouble()
				<< "\n\tCharField = '"		<< row[k * 16 + 8]->asChar()
	 			<< "'\n\tBoolField = "		<< row[k * 16 + 9]->asBool()
				<< "\n\tStringField = '"	<< row[k * 16 + 10]->asString()
				<< "'\n\tBlobField = "		<< row[k * 16 + 11]->asBlob()
				<< "\tDateField = "		<< row[k * 16 + 12]->asDate()
				<< "\n\tTimeField = "		<< row[k * 16 + 13]->asTime()
				<< "\n\tDateTimeField = "	<< row[k * 16 + 14]->asDateTime()
				<< "\n\tNumericField = "	<< row[k * 16 + 15]->asNumeric() << std::endl;
		}


		PRINT_INFO_BEGIN("Drop table")
		connection->dropObject(table1.get());
		PRINT_INFO_END
		PRINT_INFO("\n")

	} catch( const DB::XDBError &err ) {
		return;
	}
}

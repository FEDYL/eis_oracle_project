#include "Tests.h"
#include "libdb/oracle/field.hpp"
#include <vector>
#include <iostream>
/*
std::string createInsertQuery(int _long, unsigned int _ulong, short _short, unsigned short _ushort, long long _longlong, unsigned long long _ulonglong, 
				float _float, double _double, std::string _char, bool _bool, std::string _str, std::string _blob,
				std::string _date, std::string _time, std::string _datetime, double _numeric) {
	std::stringstream ss;
	ss << "INSERT INTO Table1 (" 
	<< "LongField, " << "ULongField, " << "ShortField, " << "UShortField, " << "LongLongField, " << "ULongLongField, "
	<< "FloatField, " << "DoubleField, " << "CharField, "
	<< "BoolField, " << "StringField, "
	<< "BlobField, " << "DateField, " << "TimeField, " << "DateTimeField, " << "NumericField) VALUES ("
	<< _long << ", " << _ulong << ", " << _short << ", " << _ushort << ", " << _longlong << ", " << _ulonglong << ", " 
	<< _float << ", " << _double << ", " << _char << ", "
	<< (int)_bool << ", "  << _str << ", " 
	<< _blob << ", " << _date << ", " << _time << ", " << _datetime << ", " << _numeric << ")";
	return ss.str();
}
*/
void testDataBase() {
	DB_Oracle::Factory::createInstance();
	DB::Query query;
	PRINT_INFO("\nTEST DATA BASE")
	try {
		PRINT_INFO_BEGIN("Connection") 
		DB::Connection_ptr connection = DB::Factory::getInstance()->createNoLogConnection("", USER, PASS);
                PRINT_INFO_END

		PRINT_INFO_BEGIN("Drop old table")
		bool flag = false;
		try {
			query = "DROP TABLE Table1";
			connection->execSQL(query);
			query.clear();
		} catch (const DB::XDBError &x) {
			flag = true;
		}
		if(flag) PRINT_INFO("no")
		else PRINT_INFO("yes")

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
		query = "INSERT INTO Table1 (LongField, ULongField, ShortField, UShortField, LongLongField, ULongLongField, FloatField, DoubleField, CharField, BoolField,  StringField, BlobField, DateField, NumericField) VALUES (1, 2000, -12, 2332, -1234543, 323434, 3.4, 34532.463, 'e', 1, 'One', '0', to_date('2009-10-20','yyyy-mm-dd'), 123.12)";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END
		
		PRINT_INFO_BEGIN("Create insert 2nd row query")
		query = "INSERT INTO Table1 (LongField, ULongField, ShortField, UShortField, LongLongField, ULongLongField, CharField, BoolField,  StringField, BlobField, DateField, TimeField, DateTimeField) VALUES (2, 0, -2, 123, 9887, 999826, '0', 0, 'Two', '0', to_date('1990-01-01','yyyy-mm-dd'), to_timestamp('9:00', 'hh:mi'), to_timestamp('1990-01-01 08:00', 'yyyy-mm-dd hh:mi'))";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END
		
		PRINT_INFO_BEGIN("Create insert 3rd row query")
		query = "INSERT INTO Table1 (LongField, DateField, TimeField, DateTimeField) VALUES (3, to_date('1997-12-31','yyyy-mm-dd'), to_timestamp('3:00:01', 'hh:mi:ss'), to_timestamp('1990-1-01 10:00:01', 'yyyy-mm-dd hh:mi:ss'))";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Create insert 4th row query")
		query = "INSERT INTO Table1 (LongField, ULongField, BoolField, NumericField) VALUES (4, 12, 0, 12.3)";
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Query to base")
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END


		PRINT_INFO_BEGIN("Create select query")
		std::stringstream ss;
		ss << "SELECT " << "LongField, " << "ULongField, " << "ShortField, " << "UShortField, " << "LongLongField, " << "ULongLongField, "
		<< "FloatField, " << "DoubleField, " << "BoolField, " /* char */ << "BoolField, " << "BoolField, " /* string */ << "BlobField, " 
		<< "DateField, " << "TimeField, " << "DateTimeField, " << "NumericField FROM Table1";
		query = ss.str();
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Query to base")
		DB::AutoResultSet rs(connection->execSQL(query));
		PRINT_INFO_END

		PRINT_INFO("Result of select:")
		unsigned int k = 0;
		for(DB::ResultSet::iterator i = rs->begin(); i != rs->end(); ++k, ++i) {
			std::cout << "row #" << k + 1 << ":" << std::endl;
			DB::ResultRow &row = *i;

			std::cout << "\tLongField = ";
			if(row[0]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[0]->asLong() << std::endl;

			std::cout << "\tULongField = ";
			if(row[1]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[1]->asULong() << std::endl;

			std::cout << "\tShortField = ";
			if(row[2]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[2]->asShort() << std::endl; 

			std::cout << "\tUShortField = ";
			if(row[3]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[3]->asUShort() << std::endl;

			std::cout << "\tLongLongField = ";
			if(row[4]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[4]->asLongLong() << std::endl;

			std::cout << "\tULongLongField = ";
			if(row[5]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[5]->asULongLong() << std::endl;

			std::cout << "\tFloatField = ";
			if(row[6]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[6]->asFloat() << std::endl;

			std::cout << "\tDoubleField = ";
			if(row[7]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[7]->asDouble() << std::endl;

			std::cout << "\tBoolField = ";
			if(row[8]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[8]->asBool() << std::endl; // <- char

			std::cout << "\tBoolField = ";
			if(row[9]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[9]->asBool() << std::endl;

			std::cout << "\tBoolField = ";
			if(row[10]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[10]->asBool() << std::endl; // <- string

			std::cout << "\tBlobField = ";
			if(row[11]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[11]->asBlob() << std::endl;

			std::cout << "\tDateField = ";
			if(row[12]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[12]->asDate() << std::endl;

			std::cout << "\tTimeField = ";
			if(row[13]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[13]->asTime() << std::endl;

			std::cout << "\tDateTimeField = ";
			if(row[14]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[14]->asDateTime() << std::endl;
			
			std::cout << "\tNumericField = ";
			if(row[15]->isNull()) std::cout << "NULL" << std::endl; 
			else std::cout << row[15]->asNumeric() << std::endl;
		}
/*
		DB_Oracle::ORACLE_FIELD f;
		f.name = "olol";
		f.table = "uu";
		f.precision = 5;
		f.scale = 3;
		DB_Oracle::NumberField * field = new DB_Oracle::NumberField(f, 5);
		//field->asFloat();
		field->asChar();
		delete field;
*/
		PRINT_INFO_BEGIN("Drop table")
		connection->dropObject(table1.get());
		PRINT_INFO_END
		PRINT_INFO("\n")

	} catch (const DB::XDBError &x) {
		std::cout << "no\nOops! " << x << '\n' << std::endl;
	}
}

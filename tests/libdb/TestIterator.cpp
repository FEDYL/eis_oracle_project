#include "Tests.h"

struct DATA {
	DATA() : _id_1(0), _id_2(0), _id_3(0), _snum_1(0), _snum_2(0), _snum_3(0), _ch_1(0), _ch_2(0), _ch_3(0) { }

	void setFirstRow(int id, unsigned int simpleNumber, char ch) {
		//std::cout << "\trow #1: (" << id << ", " << simpleNumber << ", '" << ch << "')" << std::endl;
		_id_1 = id;
		_snum_1 = simpleNumber;
		_ch_1 = ch;
	}
	void setSecondRow(int id, unsigned int simpleNumber, char ch) {
		//std::cout << "\trow #2: (" << id << ", " << simpleNumber << ", '" << ch << "')" << std::endl;
		_id_2 = id;
		_snum_2 = simpleNumber;
		_ch_2 = ch;
	}
	void setThirdRow(int id, unsigned int simpleNumber, char ch) {
		//std::cout << "\trow #3: (" << id << ", " << simpleNumber << ", '" << ch << "')" << std::endl;
		_id_3 = id;
		_snum_3 = simpleNumber;
		_ch_3 = ch;
	}
	
	bool equals(const DATA &d) const {
		return ( _id_1 == d._id_1 && _id_2 == d._id_2 && _id_3 == d._id_3 &&
			 _snum_1 == d._snum_1 && _snum_2 == d._snum_2 && _snum_3 == d._snum_3 &&
			 _ch_1 == d._ch_1 && _ch_2 == d._ch_2 && _ch_3 == d._ch_3 );
	}
	
	void clear() { _id_1 = _id_2 = _id_3 = _snum_1 = _snum_2 = _snum_3 = _ch_1 = _ch_2 = _ch_3 = 0; }		
private:
	int _id_1, _id_2, _id_3;
	unsigned int _snum_1, _snum_2, _snum_3;
	char _ch_1, _ch_2, _ch_3;
};

bool isAllDataEquals(const std::vector<DATA> &data) {
	unsigned int _size = data.size() - 1;
	for(unsigned int i = 0; i < _size; ++i)
		if(!data[i].equals(data[i + 1]))
			return false;
	return true;
}

void testIterator() {
	std::vector<DATA> data;
	DATA d, f, g;

	DB_Oracle::Factory::createInstance();
	DB::Query query;
	PRINT_INFO("\nTEST ITERATOR")
	try {
		PRINT_INFO_BEGIN("Connection") 
		DB::Connection_ptr connection = DB::Factory::getInstance()->createNoLogConnection(HOST, USER, PASS);
                PRINT_INFO_END


		PRINT_INFO_BEGIN("Drop old table 'IteratorTest'")
		bool flag = false;
		try {
			query = "DROP TABLE IteratorTest";
			connection->execSQL(query);
			query.clear();
		} catch (const DB::XDBError &x) {
			flag = true;
		}
		if(flag) PRINT_INFO("no")
		else PRINT_INFO("yes")

		/*
		PRINT_INFO_BEGIN("Create base")
		connection->createDataBase("libdbtest");
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Use Base")
		connection->useDataBase("libdbtest");
		PRINT_INFO_END
		*/

		PRINT_INFO_BEGIN("Create table 'IteratorTest'")
		DB::Table_ptr table = DB::Factory::getInstance()->createTable("IteratorTest");
		table->addFieldDef( new DB_Oracle::LongFieldDef("id") );
		table->addFieldDef( new DB_Oracle::LongFieldDef("simple_number") );
		table->addFieldDef( new DB_Oracle::CharFieldDef("symbol") );
		table->addPrimaryKey().addFieldDef("id");
		PRINT_INFO_END
		PRINT_INFO_BEGIN("Add table 'IteratorTest' to base")
		connection->createObject(table.get());
		PRINT_INFO_END

		PRINT_INFO_BEGIN("Insert rows to table")
		query = "INSERT INTO IteratorTest (id, simple_number, symbol) VALUES (1, 23, 'O')";
		connection->execSQL(query);
		query.clear();
		query = "INSERT INTO IteratorTest (id, simple_number, symbol) VALUES (2, 38, 'W')";
		connection->execSQL(query);
		query.clear();
		query = "INSERT INTO IteratorTest (id, simple_number, symbol) VALUES (3, 89, 'e')";
		connection->execSQL(query);
		query.clear();
		PRINT_INFO_END

		query = "SELECT * FROM IteratorTest";
		DB::AutoResultSet rs( connection->execSQL(query) );

		PRINT_INFO_BEGIN("Create iterators")
		unsigned int k = 0;
		for(DB::ResultSet::iterator i = rs->begin(); i != rs->end(); ++i, ++k) {
			DB::ResultRow &row = *i;
			switch(k) {
			case 0: d.setFirstRow(row[0]->asLong(), row[1]->asULong(), row[2]->asChar()); break;
			case 1: d.setSecondRow(row[0]->asLong(), row[1]->asULong(), row[2]->asChar()); break;
			case 2: d.setThirdRow(row[0]->asLong(), row[1]->asULong(), row[2]->asChar());
			}
		}
		data.push_back(d);
		d.clear();

		k = 0;
		for(DB::ResultSet::iterator j = rs->begin(); j != rs->end(); ++j, ++k) {
			DB::ResultRow &row = *j;
			switch(k) {
			case 0: d.setFirstRow(row[0]->asLong(), row[1]->asULong(), row[2]->asChar()); break;
			case 1: d.setSecondRow(row[0]->asLong(), row[1]->asULong(), row[2]->asChar()); break;
			case 2: d.setThirdRow(row[0]->asLong(), row[1]->asULong(), row[2]->asChar());
			}
		}
		data.push_back(d);
		d.clear();
		PRINT_INFO_END
		
		if(isAllDataEquals(data)) PRINT_INFO("All iterators getting equals data")
		else PRINT_INFO("Iterators getting not equals data")


		PRINT_INFO_BEGIN("Drop table")
		connection->dropObject(table.get());
		PRINT_INFO_END
		/*
		PRINT_INFO_BEGIN("Drop base")
		connection->dropDataBase("libdbtest");	
		PRINT_INFO_END
		PRINT_INFO("\n")
		*/
		PRINT_INFO("\n")
	} catch(const DB::XDBError &x) {
		std::cout << "no\nOops! " << x << '\n' << std::endl;
	}
}

#ifdef WIN32
  #include <Winsock2.h>
#endif

#include "occi.h"

#include <iostream>
#include <cassert>
#include <typeinfo>
#include <vector>

#include "libdb/oracle/resultset.hpp"
#include "libdb/oracle/field.hpp"

using namespace oracle::occi;

DB_Oracle::ResultSet::ResultSet(oracle::occi::Connection * connection, std::string query) : m_connection(connection), m_query(query) {
	if(!connection)
		throw DB::XDBError("Connection is incorrect.");
	checkIterators(0);
	m_numRows = calcSize(query);	
}

DB_Oracle::ResultSet::~ResultSet() {
	for(Iterators::iterator i = m_iterators.begin(); i != m_iterators.end(); ++i) {
		(*i)->closeResultSet(m_stmt);
		delete *i;
	}
	m_connection->terminateStatement(m_stmt);	
}

DB::ResultSet::size_type DB_Oracle::ResultSet::size() const {
	return (DB::ResultSet::size_type)m_numRows;
}

unsigned int DB_Oracle::ResultSet::calcSize(const std::string &query) {
	unsigned int num = 0;
	std::string str(query.c_str());
	std::string::size_type loc1 = str.find(" FROM", 0);
	str.replace(0, loc1, "SELECT count(*)");
	try {
		oracle::occi::Statement * stmt = m_connection->createStatement(str);		
		oracle::occi::ResultSet * rset = stmt->executeQuery();

		rset->next();
		num = rset->getInt(1);
		stmt->closeResultSet(rset);
		m_connection->terminateStatement(stmt);
	} catch (const SQLException &x) {
		throw DB::XDBError("Couldn't calc size!");
	}
	return num;
}

void DB_Oracle::ResultSet::fetchFields() {
	try {
		std::vector<MetaData> listOfColumns = m_iterator->resultSet()->getColumnListMetaData();	
		m_fields.clear();
		m_numCols = listOfColumns.size();
		ORACLE_FIELD f;
		for(unsigned int i = 0; i < m_numCols; ++i) {
			MetaData md = (MetaData)listOfColumns[i];
			f.name = md.getString(MetaData::ATTR_NAME);			//!< - имя поля.
			f.type = md.getInt(MetaData::ATTR_DATA_TYPE);			//!< - тип поля.
			if(f.type == ORACLE_FIELD::FIELD_TYPE_NUMBER) {
				f.precision = md.getInt(MetaData::ATTR_PRECISION);	//!< - разрядность.
				f.scale = md.getInt(MetaData::ATTR_SCALE); 		//!< - количество знаков после запятой.
			}
			f.length = md.getInt(MetaData::ATTR_DATA_SIZE);			//!< - размер поля.
			m_fields.push_back(f);
		}
	} catch(const SQLException &x) {
		throw DB::XDBError("Fetch fields error!");
	}
}


void DB_Oracle::ResultSet::next(difference_type pos) {
	if(eof(pos)) 
		return;
	m_iterator->next();
}

void DB_Oracle::ResultSet::checkIterators(unsigned int pos) {
	for(Iterators::iterator i = m_iterators.begin(); i != m_iterators.end(); ++i)
		if(pos == (*i)->position()) {
			m_iterator = (*i);
			return; 
		}
	if(pos != 0)
		throw(DB::XDBError("Requisting position is invalid"));
	m_stmt = m_connection->createStatement(m_query);
	m_iterator = new InternalIterator(m_stmt->executeQuery());
	m_iterators.push_back(m_iterator);
}

void DB_Oracle::ResultSet::getData(difference_type pos, DB::ResultRow & atrow) {
	if((size_type)pos > size())
		throw(DB::XDBError("Requisting position is invalid"));

	if(eof(pos)) return;

	if((size_type)pos != m_iterator->position())
		checkIterators(pos);

	DB::Field * field;
	bool nulldata;
	if(atrow.size() == 0) {
		fetchFields();
		for(unsigned int i = 0; i < m_numCols; ++i) {
			nulldata = m_iterator->resultSet()->isNull(i + 1);
			switch(m_fields[i].type) {
			case ORACLE_FIELD::FIELD_TYPE_NUMBER:
				if(nulldata)
					field = new NumberField(m_fields[i]);
				else
					field = new NumberField(m_fields[i], m_iterator->resultSet()->getNumber(i + 1));
				break;
			case ORACLE_FIELD::FIELD_TYPE_TIMESTAMP:
				if(nulldata)
					field = new TimestampField(m_fields[i]);
				else
					field = new TimestampField(m_fields[i], m_iterator->resultSet()->getTimestamp(i + 1));
				break;
			case ORACLE_FIELD::FIELD_TYPE_STRING:
				if(nulldata)
					field = new StringField(m_fields[i]);
				else
					field = new StringField(m_fields[i], m_iterator->resultSet()->getString(i + 1));
				break;
			case ORACLE_FIELD::FIELD_TYPE_BLOB:
				if(nulldata)
					field = new BlobField(m_fields[i]);
				else
					field = new BlobField(m_fields[i], m_iterator->resultSet()->getBlob(i + 1));
				break;
			default:
				throw DB::XDBError("Unsupported field type!");
			}
			atrow.push(m_fields[i].name, field);
		}
	} else {
		for(unsigned int i = 0; i < m_numCols; ++i) {
			nulldata = m_iterator->resultSet()->isNull(i + 1);
			field = atrow.getByIndex_NoConst(i);
			if(nulldata) {
				((Field *)field)->setNull();
				continue;
			}
			switch(m_fields[i].type) {
			case ORACLE_FIELD::FIELD_TYPE_NUMBER:
				if(typeid(*field) == typeid(NumberField))
					((NumberField *)field)->setData(m_iterator->resultSet()->getNumber(i + 1));
				else
					throw DB::XDBError("Incorrect result row!");
				break;
			case ORACLE_FIELD::FIELD_TYPE_TIMESTAMP:
				if(typeid(*field) == typeid(TimestampField))
					((TimestampField *)field)->setData(m_iterator->resultSet()->getTimestamp(i + 1));
				else
					throw DB::XDBError("Incorrect result row!");
				break;
			case ORACLE_FIELD::FIELD_TYPE_STRING:
				if(typeid(*field) == typeid(StringField))
					((StringField *)field)->setData(m_iterator->resultSet()->getString(i + 1));
				else
					throw DB::XDBError("Incorrect result row!");
				break;
			case ORACLE_FIELD::FIELD_TYPE_BLOB:
				if(typeid(*field) == typeid(BlobField))
					((BlobField *)field)->setData(m_iterator->resultSet()->getBlob(i + 1));
				else
					throw DB::XDBError("Incorrect result row!");
				break;
			default:
				throw DB::XDBError("Unsupported field type!");
			}
		}
	}
}

bool DB_Oracle::ResultSet::eof(difference_type pos) const {
	return ((DB::ResultSet::size_type)pos >= size());
}

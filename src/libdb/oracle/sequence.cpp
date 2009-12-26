#ifdef WIN32
  #include <Winsock2.h>
#endif

#include "occi.h"
#include "libdb/common/query.hpp"
#include "libdb/common/resultset.hpp"
#include "libdb/oracle/sequence.hpp"
#include "libdb/oracle/internalconnection.hpp"
#include "libdb/oracle/transaction.hpp"

DB_Oracle::Sequence::Sequence(const std::string & name, const unsigned int initial_value) : DB::Sequence(name, initial_value), m_value(0) { }

DB_Oracle::Sequence::Sequence(DB::InternalConnection * connection, const std::string & name, const unsigned int initial_value)
	: DB::Sequence(connection, name, initial_value), m_value(0) { }

unsigned long long DB_Oracle::Sequence::getNextVal(unsigned int increment) {
	if(!m_connection)
		throw DB::XDBError("DB_Oracle::Sequence::getNextVal(): counldn't return the value because there is no database connection.");
	
	DB::Query query;

	Transaction tr(m_connection);

	query << "SELECT Value FROM eis_SequenceValue WHERE Name = \"" << m_name << "\" FOR UPDATE";
	DB::AutoResultSet res(m_connection->execSQL(query));
	query.clear();
	DB::ResultSet::iterator i = res->begin();
	if(i == res->end())
		throw XNotFound(m_name);
	else {
		DB::ResultRow & row = *i;
		if(row[0]->isNull())
			throw XNotFound(m_name);
		else {
			m_value = row[0]->asULongLong() + increment;
			query << "UPDATE eis_SequenceValue SET Value = " << m_value << " WHERE Name = \"" << m_name << '\"';
			m_connection->execSQL(query);
		}
	}
	tr.commit();
	return m_value;
}

unsigned long long DB_Oracle::Sequence::getValue() {
	if(m_value == 0)
		return getNextVal();
	else
		return m_value;
}

std::string DB_Oracle::Sequence::str() const {
	std::ostringstream strvalue;
	strvalue << m_value;
	return strvalue.str();
}

void DB_Oracle::Sequence::create(DB::InternalConnection * connection, bool if_not_exists) {
	m_connection = connection;
	DB::Query query;
	query << "SELECT Value FROM eis_SequenceValue WHERE Name = \"" << m_name << '\"';
	DB::AutoResultSet res(m_connection->execSQL(query));
	if(res->begin() == res->end()) {
		query.clear();
		query << "INSERT INTO eis_SequenceValue (Name, Value) VALUES (\"" << m_name << "\", " << initial_value_ << ')';
		m_connection->execSQL(query);
	} else {
		if(if_not_exists)
			return;
		DB::XDBError err;
		err << "Sequence with thae name \"" << m_name << "\" already exists in database.";
		throw(err);
	}
}

void DB_Oracle::Sequence::drop(bool if_exists) {
	DB::Query query;
	query << "DELETE FROM eis_SequenceValue WHERE Name = \"" << m_name << '\"';
	m_connection->execSQL(query);
}

void DB_Oracle::Sequence::drop(DB::InternalConnection * connection, bool if_exists) {
	m_connection = connection;
	drop();
}

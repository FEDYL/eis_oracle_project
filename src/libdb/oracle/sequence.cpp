#ifdef WIN32
  #include <Winsock2.h>
#endif

#include "occi.h"
#include "libdb/common/query.hpp"
#include "libdb/common/resultset.hpp"
#include "libdb/oracle/sequence.hpp"
#include "libdb/oracle/internalconnection.hpp"
#include "libdb/oracle/transaction.hpp"

DB_Oracle::Sequence::Sequence(DB::InternalConnection * connection, const std::string & name, const unsigned int initial_value)
	: DB::Sequence(connection, name, initial_value), m_value(0) {
	create(connection);
}

unsigned long long DB_Oracle::Sequence::getNextVal(unsigned int increment) {	
	DB::Query query;
	try {
		Transaction tr(m_connection);
		if(increment > 1) {
			query << "ALTER SEQUENCE " << m_name << " INCREMENT BY " << increment;
			m_connection->execSQL(query);
			query.clear();
		}
		query << "SELECT " << m_name << ".nextval FROM DUAL";
		DB::AutoResultSet res(m_connection->execSQL(query));
		query.clear();
		DB::ResultSet::iterator i = res->begin();
		if(i == res->end())
			throw XNotFound(m_name);
		else {
			DB::ResultRow & row = *i;
			if(row[0]->isNull())
				throw XNotFound(m_name);
			else
				m_value = row[0]->asULongLong();
		}
		if(increment > 1) {
			query << "ALTER SEQUENCE " << m_name << " INCREMENT BY 1";
			m_connection->execSQL(query);
			query.clear();
		}
		tr.commit();
	} catch (const oracle::occi::SQLException &x) {
		std::stringstream stream;
		stream << "Getting next value of sequence error.\nMessage: " << x.what() << std::endl;
		std::string error_code = stream.str();
		throw DB::XDBError(error_code);
	} catch (const XNotFound &x) {
		std::stringstream stream;
		stream << "Not found sequence: " << x.sequence_name() << std::endl;
		std::string error_code = stream.str();
		throw DB::XDBError(error_code);
	}
	return m_value;
}

unsigned long long DB_Oracle::Sequence::getValue() {
	return m_value;
}

std::string DB_Oracle::Sequence::str() const {
	std::stringstream stream;
	stream << m_value;
	return stream.str();
}

void DB_Oracle::Sequence::create(DB::InternalConnection * connection, bool if_not_exists) {
	m_connection = connection;
	DB::Query query;
	try {
		query << "CREATE SEQUENCE " << m_name << "\n  MINVALUE " << initial_value_
			<< "\n  START WITH " << initial_value_ << "\n  INCREMENT BY 1\n  CACHE 20";
		m_connection->execSQL(query);
	} catch (const oracle::occi::SQLException &x) {
		if(if_not_exists)
			return;
		std::stringstream stream;
		stream << "Creating sequence error.\nMessage: " << x.what() << std::endl;
		std::string error_code = stream.str();
		throw DB::XDBError(error_code);
	}
}

void DB_Oracle::Sequence::drop(bool if_exists) {
	DB::Query query;
	try {
		query << "DROP SEQUENCE " << m_name;
		m_connection->execSQL(query);
	} catch (const oracle::occi::SQLException &x) {
		if(!if_exists)
			return;
		std::stringstream stream;
		stream << "Droping sequence error.\nMessage: " << x.what() << std::endl;
		std::string error_code = stream.str();
		throw DB::XDBError(error_code);
	}
}

void DB_Oracle::Sequence::drop(DB::InternalConnection * connection, bool if_exists) {
	m_connection = connection;
	drop();
}

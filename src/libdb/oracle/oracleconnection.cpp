#ifdef WIN32
  #include <Winsock2.h>
#endif

#include <iostream>

#include "occi.h"

#include "libdb/oracle/oracleconnection.hpp"
#include "libdb/oracle/resultset.hpp"
#include "libdb/common/xdberror.hpp"

DB_Oracle::OracleConnection * DB_Oracle::OracleConnection::createConnection(std::string user, std::string password, std::string url) {
	OracleConnection * res = new OracleConnection();
	res->m_environment = Environment::createEnvironment(Environment::OBJECT);
	try {
		res->m_connection = res->m_environment->createConnection(user, password, url);
	} catch(const SQLException &x) {
		std::stringstream stream;
		stream << "Error connecting to server.\nMessage: " << x.what() << std::endl;
		std::string error_code = stream.str();
		throw DB::XDBError(error_code);
	}	
	return res;
}

void DB_Oracle::OracleConnection::terminateConnection(OracleConnection * connection) {
	connection->m_environment->terminateConnection(connection->m_connection);
	Environment::terminateEnvironment(connection->m_environment);
}

void DB_Oracle::OracleConnection::executeUpdate(std::string query) {
	try {
		if(query[query.size() - 1] == ';') 
			query.erase(query.size() - 1, 1);
		Statement * stmt = m_connection->createStatement(query);
		stmt->executeUpdate();
		m_connection->commit();
		m_connection->terminateStatement(stmt);
	} catch(const oracle::occi::SQLException &x) {
		std::stringstream stream;
		stream << "Execute query error.\nMessage: " << x.what() << "Query: " << query << std::endl;
		std::string error_code = stream.str();
		throw DB::XDBError(error_code);
	}

	
}

DB_Oracle::ResultSet * DB_Oracle::OracleConnection::executeQuery(std::string query) {
	try {
		if(query[query.size() - 1] == ';') 
			query.erase(query.size() - 1, 1);
		return new DB_Oracle::ResultSet(m_connection, query);
	} catch(const SQLException &x) {
		std::stringstream stream;
		stream << "Execute query error.\nMessage: " << x.what() << "\nQuery: " << query << std::endl;
		std::string error_code = stream.str();
		throw DB::XDBError(error_code);
	}
}

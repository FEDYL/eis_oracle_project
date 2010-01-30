#ifdef WIN32
  #include <Winsock2.h>
#endif

#include "occi.h"

#include "libdb/oracle/oracleconnection.hpp"
#include "libdb/oracle/resultset.hpp"
#include "libdb/common/xdberror.hpp"

DB_Oracle::OracleConnection * DB_Oracle::OracleConnection::createConnection(std::string user, std::string password, std::string url) {
	OracleConnection * res = new OracleConnection();
	res->m_environment = Environment::createEnvironment(Environment::DEFAULT);
	try {
		res->m_connection = res->m_environment->createConnection(user, password, url);
	} catch(SQLException x) {
		std::cout << "\nOracleConnection: couldn't connect with server.\nMessage: " << x.getMessage();
		throw DB::XDBError("OracleConnection: couldn't connect with server.");
	}	
	return res;
}

void DB_Oracle::OracleConnection::terminateConnection(OracleConnection * connection) {
	connection->m_environment->terminateConnection(connection->m_connection);
	Environment::terminateEnvironment(connection->m_environment);
	delete connection;
}

void DB_Oracle::OracleConnection::executeUpdate(std::string query) {
	try {
		if(query[query.size() - 1] == ';') 
			query.erase(query.size() - 1, 1);
		Statement * stmt = m_connection->createStatement(query.c_str());
		stmt->executeUpdate();
		m_connection->commit();
		m_connection->terminateStatement(stmt);
	} catch(SQLException x) {
		std::cout << "\nOracleConnection: execute update exception.\nMessage: " << x.getMessage()
			  << "Query: " << query << std::endl;
		throw DB::XDBError("OracleConnection: execute update exception.");
	}
}

DB_Oracle::ResultSet * DB_Oracle::OracleConnection::executeQuery(std::string query) {
	try {
		if(query[query.size() - 1] == ';') 
			query.erase(query.size() - 1, 1);
		return new DB_Oracle::ResultSet(m_connection, query);
	} catch(SQLException x) {
		std::cout << "\nOracleConnection: execute query exception.\nMessage: " << x.getMessage()
			  << "Query: " << query << std::endl;
		throw DB::XDBError("OracleConnection: execute query exception.");
	}
}

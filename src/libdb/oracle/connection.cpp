#ifdef WIN32
  #include <Winsock2.h>
#endif

#include "boost/xpressive/xpressive.hpp"

#include "occi.h"

#include "libdb/oracle/oracleconnection.hpp"
#include "libdb/oracle/field.hpp"
#include "libdb/oracle/sequence.hpp"
#include "libdb/oracle/connection.hpp"
#include "libdb/oracle/resultset.hpp"
#include "libdb/oracle/internalconnection.hpp"
#include "libdb/oracle/transaction.hpp"

#include "libdb/common/ConnectionPool.hpp"
#include "libdb/common/table.hpp"
#include "libdb/common/xdberror.hpp"
#include "libdb/common/logger.hpp"

boost::xpressive::sregex rex_const = boost::xpressive::sregex::compile("^select.*", boost::xpressive::regex_constants::icase);
boost::xpressive::sregex rex_noconst = boost::xpressive::sregex::compile("^(insert|update|delete|create|drop|alter).*", boost::xpressive::regex_constants::icase);

//------------------------------------------------------------------------------------------------------
// class : Logger
//------------------------------------------------------------------------------------------------------

static Logger::Logger * logger = NULL;

void DB::setLogger(Logger::Logger* value) {
	logger = value;
}



//------------------------------------------------------------------------------------------------------
// class : InternalConnection
//------------------------------------------------------------------------------------------------------

DB_Oracle::InternalConnection::InternalConnection(const char * host, const char * user, const char * pass, const char * dbname) : DB::InternalConnection() {
	if(strlen(dbname))
		throw DB::XDBError("Data base name is not null.");
	m_orclConn = OracleConnection::createConnection(user, pass, host);
}

DB_Oracle::InternalConnection::~InternalConnection() {
	OracleConnection::terminateConnection(m_orclConn);
}

void DB_Oracle::InternalConnection::execConstSQL(const DB::Query & query) {
	executeScript(query.str());
}

void DB_Oracle::InternalConnection::executeScript(const std::string & script) {
	if(logger != NULL) {
		MT::SingleLock lock(m_mutex);
		*logger << LOG_INFO << "Thread " << pthread_self() << ' ' << script;
	}
	m_orclConn->executeUpdate(script);
}

DB::Sequence * DB_Oracle::InternalConnection::createSequence(const std::string & name, const unsigned int initial_value) {
	return new Sequence(this, name, initial_value);
}

void DB_Oracle::InternalConnection::getTableStructureFromDB(DB::Table * table) {
	throw DB::XDBError("Operation is not supported: InternalConnection::getTableStructureFromDB()");
}

void DB_Oracle::InternalConnection::createDataBase(const std::string & name) {
	throw DB::XDBError("Operation is not supported: InternalConnection::createDataBase()");
}

void DB_Oracle::InternalConnection::dropDataBase(const std::string & name) {
	throw DB::XDBError("Operation is not supported: InternalConnection::dropDataBase()");
}

void DB_Oracle::InternalConnection::useDataBase(const std::string & name) {
	throw DB::XDBError("Operation is not supported: InternalConnection::useDataBase()");
}

bool DB_Oracle::InternalConnection::ForeignKey::correspondIndex(Index & index) {
	References::iterator r_it = references.begin();
	Index::FieldDefs::iterator f_it = index.fields.begin();
	while(r_it != references.end()) {
		if(r_it->field != *f_it)
			return false;
		r_it++;
		f_it++;
	}
	if(f_it != index.fields.end())
		return false;
	return true;
}

//------------------------------------------------------------------------------------------------------
// class : InternalLogConnection
//------------------------------------------------------------------------------------------------------

DB::ResultSet * DB_Oracle::InternalLogConnection::execSQL(const DB::Query & query) {
	bool modificating = true;
	std::string sql(query.str());
	MT::SingleLock lock(m_mutex);

	if(boost::xpressive::regex_match(sql, rex_const))
		modificating = false;
	else if(!boost::xpressive::regex_match(sql, rex_noconst)) {
		DB::XDBError err; 
		err << "Unknown query:\n" << sql;
		throw err;
	}
	
	if(modificating) {
#ifdef ENABLE_QUERY_LOG
		checkFailedTransactions(sql);
		Transaction tr(this);

		// создание лога.

#endif		
		execConstSQL(sql);
#ifdef ENABLE_QUERY_LOG
		tr.commit();
#endif		
		return NULL;
	} else {
		return m_orclConn->executeQuery(sql);
	}
}



//------------------------------------------------------------------------------------------------------
// class : InternalNoLogConnection
//------------------------------------------------------------------------------------------------------

DB::ResultSet * DB_Oracle::InternalNoLogConnection::execSQL(const DB::Query & query) {
	bool modificating = true;
	std::string sql(query.str());
	MT::SingleLock lock(m_mutex);

	if(boost::xpressive::regex_match(sql, rex_const))
		modificating = false;
	else if(!boost::xpressive::regex_match(sql, rex_noconst)) {
		DB::XDBError err; 
		err << "Unknown query:\n" << sql;
		throw err;
	}
	if(modificating) {
		execConstSQL(sql);
		return NULL;
	} else
		return m_orclConn->executeQuery(sql);
}

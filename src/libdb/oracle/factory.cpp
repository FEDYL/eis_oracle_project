#ifdef WIN32
  #include <Winsock2.h>
#endif

#include "libdb/oracle/factory.hpp"
#include "libdb/oracle/connection.hpp"
#include "libdb/oracle/sequence.hpp"
#include "libdb/common/xdberror.hpp"

void DB_Oracle::Factory::createInstance() {
	if(m_factory)
		throw DB::XDBError("The process already has instance of factory.");
	m_factory = new Factory();
}

DB::Connection * DB_Oracle::Factory::createLogConnection(const char * host, const char * user, const char * pass, const char * dbname ) {
	return( new LogConnection(host, user, pass, dbname) );
}

DB::Connection * DB_Oracle::Factory::createNoLogConnection(const char * host, const char * user, const char * pass, const char * dbname) {	
	return( new NoLogConnection(host, user, pass, dbname) );
}

DB::Sequence * DB_Oracle::Factory::createSequence(const std::string & name, unsigned int initial_value) {
	throw DB::XDBError("Unsupported operation: Factory::createSequence()");
	return NULL;
}

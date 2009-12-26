#ifdef WIN32
  #include <Winsock2.h>
#endif

#include "transaction.hpp"

DB_Oracle::Transaction::Transaction(DB::InternalConnection * connection) : m_connection(connection), m_lock(connection->getMutex()), m_is_commited(false) {
	if(!m_connection->isInTransaction()) {
		DB::Query begin("BEGIN");
		m_connection->execConstSQL(begin);
	}
	m_connection->m_transaction_count++;
	m_connection->m_transaction_rolled_back = false;
}

DB_Oracle::Transaction::~Transaction() {
	if(!m_is_commited) {
		DB::Query rollback("ROLLBACK");
		m_connection->execConstSQL(rollback);
		m_connection->m_transaction_count--;
		m_connection->m_transaction_rolled_back = true;
	}
}

void DB_Oracle::Transaction::commit() {
	m_connection->m_transaction_count--;
	if(!m_connection->isInTransaction()) {
		DB::Query commit("COMMIT");
		m_connection->execConstSQL(commit);
	}
	m_is_commited = true;
}

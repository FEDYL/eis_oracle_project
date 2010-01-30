#ifndef __DB_Oracle_ResultSetHPP__
#define __DB_Oracle_ResultSetHPP__

#include "occi.h"

#include <vector>

#include "libdb/common/resultset.hpp"
#include "libdb/common/xdberror.hpp"
#include "libdb/oracle/field.hpp"

namespace DB_Oracle {

//! Резульат запроса.
/*!
  Реализация интерфейса DB::ResultSet.
 */
class ResultSet : public DB::ResultSet {
public:
	//! Конструктор.
	/*!
	  \param connection - поключение к базе данных.
	  \param query - запрос.
	  Объект создается из результата запроса resultSet, возвращаемого клиентской библиотекой Oracle.
	 */
	ResultSet(oracle::occi::Connection * connection, std::string query) {
		if(!connection)
			throw DB::XDBError("Connection is incorrect.");
		m_connection = connection;		
		m_stmt = m_connection->createStatement(query);
		m_resultSet = m_stmt->executeQuery();
		m_numRows = calcSize(query);	
	}
	//! Деструктор.
	virtual ~ResultSet() {
		std::cout << "virtual ~ResultSet();\n";
		m_stmt->closeResultSet(m_resultSet);
		m_connection->terminateStatement(m_stmt);
	}

	//! Метод считает длину, делая запрос select count(*).
	unsigned int calcSize(const std::string &query); 

	//! Реализация метода DB::ResultSet::size().
	size_type size() const {
		return (DB::ResultSet::size_type)m_numRows;
	}

private:
	//! Получает информацию о полях результата и записывает их в m_fields.
	typedef std::vector<ORACLE_FIELD> OracleFields;
	void fetchFields();

	oracle::occi::Connection * m_connection;	//!< - подключение, от которого получен результат.
	oracle::occi::Statement  * m_stmt;		//!< - запрос.
	oracle::occi::ResultSet  * m_resultSet;		//!< - результат запроса.

	unsigned int m_numRows;				//!< - количество строк в результате запроса.
	unsigned int m_numCols;				//!< - количество полей в результате запроса.

	OracleFields m_fields;				//!< - информация о полях результата.


protected:
	//! Реализация метода DB::ResultSet::next().
	virtual void next(difference_type pos);
	//! Реализация метода DB::ResultSet::getData().
	virtual void getData(difference_type pos, DB::ResultRow & row);
	//! Реализация метода DB::ResultSet::eof().
	virtual bool eof(difference_type pos) const {return (pos==m_numRows);}
};

} // namespace DB_Oracle

#endif //__DB_Oracle_ResultSetHPP__

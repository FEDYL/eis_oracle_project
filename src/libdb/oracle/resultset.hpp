#ifndef __DB_Oracle_ResultSetHPP__
#define __DB_Oracle_ResultSetHPP__

#include "occi.h"

#include <vector>

#include "libdb/common/resultset.hpp"
#include "libdb/common/xdberror.hpp"
#include "libdb/oracle/field.hpp"

namespace DB_Oracle {

//! �������� �������.
/*!
  ���������� ���������� DB::ResultSet.
 */
class ResultSet : public DB::ResultSet {
public:
	//! �����������.
	/*!
	  \param connection - ���������� � ���� ������.
	  \param query - ������.
	  ������ ��������� �� ���������� ������� resultSet, ������������� ���������� ����������� Oracle.
	 */
	ResultSet(oracle::occi::Connection * connection, std::string query) {
		if(!connection)
			throw DB::XDBError("Connection is incorrect.");
		m_connection = connection;		
		m_stmt = m_connection->createStatement(query);
		m_resultSet = m_stmt->executeQuery();
		m_numRows = calcSize(query);	
	}
	//! ����������.
	virtual ~ResultSet() {
		std::cout << "virtual ~ResultSet();\n";
		m_stmt->closeResultSet(m_resultSet);
		m_connection->terminateStatement(m_stmt);
	}

	//! ����� ������� �����, ����� ������ select count(*).
	unsigned int calcSize(const std::string &query); 

	//! ���������� ������ DB::ResultSet::size().
	size_type size() const {
		return (DB::ResultSet::size_type)m_numRows;
	}

private:
	//! �������� ���������� � ����� ���������� � ���������� �� � m_fields.
	typedef std::vector<ORACLE_FIELD> OracleFields;
	void fetchFields();

	oracle::occi::Connection * m_connection;	//!< - �����������, �� �������� ������� ���������.
	oracle::occi::Statement  * m_stmt;		//!< - ������.
	oracle::occi::ResultSet  * m_resultSet;		//!< - ��������� �������.

	unsigned int m_numRows;				//!< - ���������� ����� � ���������� �������.
	unsigned int m_numCols;				//!< - ���������� ����� � ���������� �������.

	OracleFields m_fields;				//!< - ���������� � ����� ����������.


protected:
	//! ���������� ������ DB::ResultSet::next().
	virtual void next(difference_type pos);
	//! ���������� ������ DB::ResultSet::getData().
	virtual void getData(difference_type pos, DB::ResultRow & row);
	//! ���������� ������ DB::ResultSet::eof().
	virtual bool eof(difference_type pos) const {return (pos==m_numRows);}
};

} // namespace DB_Oracle

#endif //__DB_Oracle_ResultSetHPP__

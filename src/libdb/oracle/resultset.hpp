#ifndef __DB_Oracle_ResultSetHPP__
#define __DB_Oracle_ResultSetHPP__

#include "occi.h"

#include <vector>
#include <map>

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
	ResultSet(oracle::occi::Connection * connection, std::string query);

	//! ����������.
	virtual ~ResultSet();

	//! ���������� ������ DB::ResultSet::size().
	size_type size() const;

private:
	//! ����� ��������� ��������.
	/*!
	  ��� ������� ��������� �� ����� ResultSet ��������� ��������� ����� ������.
	 */
	class InternalIterator {
	public:
		//! �����������.
		InternalIterator(oracle::occi::ResultSet * resultSet) : m_resultSet(resultSet), m_position(0) {
			m_resultSet->next();
		}
		
		//! ��������� ������������ ������.
		void closeResultSet(oracle::occi::Statement * stmt) {
			if(!m_resultSet) return;
			stmt->closeResultSet(m_resultSet);
			m_resultSet = NULL;
		}
		//! ����������� �������� �� ���� ������� ������.
		unsigned int next() {
			m_position++;
			return m_resultSet->next();
		}
		//! �������� � ���������� �������.
		oracle::occi::ResultSet * resultSet() { return m_resultSet; }
		//! �������� � ������� ������� ���������.
		unsigned int position() { return m_position; }
	private:
		unsigned int m_position;		//!< - ������� ������� ���������.
		oracle::occi::ResultSet * m_resultSet;	//!< - ��������� �������.
	};


	typedef std::vector<ORACLE_FIELD> OracleFields;
	typedef std::vector<InternalIterator *> Iterators;

	//! ����� ������� �����, ����� ������ select count(*).
	/*!
	  �������� � ������� "select ... from" �� "select count(*) from".
	 */
	unsigned int calcSize(const std::string &query);

	//! �������� ���������� � ����� ���������� � ���������� �� � m_fields.
	void fetchFields();

	//! ��������� ���������.
	/*!
	  ��������� �������� ��������� �� ������������ ������� � ������ ������ ��������.
	  ���� �� ������, �� ���� ������� ��� �������� �������, �� ������� ����� ��������,
	  � ���� ������ ������ ����������, ��� ������� ��������.
	 */
	void checkIterators(unsigned int pos);

	OracleFields			m_fields;	//!< - ���������� � ����� ����������.
	Iterators			m_iterators;	//!< - ��� �������� ���������, � ��� ����� � ��������.
	InternalIterator *		m_iterator;	//!< - �������� ��������.
	oracle::occi::Connection *	m_connection;	//!< - �����������, �� �������� ������� ���������.
	oracle::occi::Statement *	m_stmt;		//!< - ������.
	std::string			m_query;	//!< - SQL-��� �������.

	unsigned int m_numRows;				//!< - ���������� ����� � ���������� �������.
	unsigned int m_numCols;				//!< - ���������� ����� � ���������� �������.

protected:
	//! ���������� ������ DB::ResultSet::next().
	virtual void next(difference_type pos);
	//! ���������� ������ DB::ResultSet::getData().
	virtual void getData(difference_type pos, DB::ResultRow & atrow);
	//! ���������� ������ DB::ResultSet::eof().
	virtual bool eof(difference_type pos) const;
};

} // namespace DB_Oracle

#endif //__DB_Oracle_ResultSetHPP__

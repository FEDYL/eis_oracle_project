#ifndef __DB_Oracle_ResultSetHPP__
#define __DB_Oracle_ResultSetHPP__

#include "occi.h"

#include <vector>
#include <map>

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
	ResultSet(oracle::occi::Connection * connection, std::string query);

	//! Деструктор.
	virtual ~ResultSet();

	//! Реализация метода DB::ResultSet::size().
	size_type size() const;

private:
	//! Класс внутрений итератор.
	/*!
	  Для каждого итератора на класс ResultSet создается экземпляр этого класса.
	 */
	class InternalIterator {
	public:
		//! Конструктор.
		InternalIterator(oracle::occi::ResultSet * resultSet) : m_resultSet(resultSet), m_position(0) {
			m_resultSet->next();
		}
		
		//! Правильно высвобождает память.
		void closeResultSet(oracle::occi::Statement * stmt) {
			if(!m_resultSet) return;
			stmt->closeResultSet(m_resultSet);
			m_resultSet = NULL;
		}
		//! Передвигает итератор на одну позицию вперед.
		unsigned int next() {
			m_position++;
			return m_resultSet->next();
		}
		//! Акцептор к результату запроса.
		oracle::occi::ResultSet * resultSet() { return m_resultSet; }
		//! Акцептор к текущей позиции итератора.
		unsigned int position() { return m_position; }
	private:
		unsigned int m_position;		//!< - текущая позиция итератора.
		oracle::occi::ResultSet * m_resultSet;	//!< - результат запроса.
	};


	typedef std::vector<ORACLE_FIELD> OracleFields;
	typedef std::vector<InternalIterator *> Iterators;

	//! Метод считает длину, делая запрос select count(*).
	/*!
	  Заменяет в запросе "select ... from" на "select count(*) from".
	 */
	unsigned int calcSize(const std::string &query);

	//! Получает информацию о полях результата и записывает их в m_fields.
	void fetchFields();

	//! Проверяет итераторы.
	/*!
	  Проверяет созданые итераторы по установленой позиции и нужный делает активным.
	  Если не найден, то если позиция для проверки нулевая, то создает новый итератор,
	  в ином случае кидает исключение, что позиция неверная.
	 */
	void checkIterators(unsigned int pos);

	OracleFields			m_fields;	//!< - информация о полях результата.
	Iterators			m_iterators;	//!< - все созданые итераторы, в том числе и активный.
	InternalIterator *		m_iterator;	//!< - активный итератор.
	oracle::occi::Connection *	m_connection;	//!< - подключение, от которого получен результат.
	oracle::occi::Statement *	m_stmt;		//!< - запрос.
	std::string			m_query;	//!< - SQL-код запроса.

	unsigned int m_numRows;				//!< - количество строк в результате запроса.
	unsigned int m_numCols;				//!< - количество полей в результате запроса.

protected:
	//! Реализация метода DB::ResultSet::next().
	virtual void next(difference_type pos);
	//! Реализация метода DB::ResultSet::getData().
	virtual void getData(difference_type pos, DB::ResultRow & atrow);
	//! Реализация метода DB::ResultSet::eof().
	virtual bool eof(difference_type pos) const;
};

} // namespace DB_Oracle

#endif //__DB_Oracle_ResultSetHPP__

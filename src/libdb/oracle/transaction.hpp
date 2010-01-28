#ifndef __DB_Oracle_TransactionHPP__
#define __DB_Oracle_TransactionHPP__

#include "libdb/common/connection.hpp"

namespace DB_Oracle {

//! Транзакция.
/*!
  Внутренняя транзакция, экземпляр которой может быть создан только объектами,
  принадежащими пространству имен DB_Oracle.
 */
class Transaction {
public:
	//! Конструктор.
	/*!
	  Создает транзакцию и начинает ее (выполняет SQL-оператор BEGIN,
	  в случае, если клиентом в данном соединении с базой не начата транзакция.
	  \param connection - соединение с базой данных.
	 */
	Transaction(DB::InternalConnection * connection);
	//! Деструктор.
	/*!
	  Перед уничтожением объекта выполняет оператор SQL-оператор ROLLBACK,
	  в случае, если клиентом не начата транзакция, и не был вызван
	  метод commit.
	 */
	~Transaction();
	
	//! Зафиксировать изменения.
	/*!
	  фиксирует изменения проведенные в базе в рамках этой транзакции
	  (вызывая SQL-оператор COMMIT), в случае, если клиентом не начата
	  транзакция.
	 */
	void commit();

private:
	DB::InternalConnection * m_connection;	//!< - указатель на внутреннее соединение с базой.
	MT::SingleLock m_lock;
	bool m_is_commited;			//!< - флаг зафиксированности изменений.
};

}  // namespace DB_Oracle

#endif //__DB_Oracle_TransactionHPP__

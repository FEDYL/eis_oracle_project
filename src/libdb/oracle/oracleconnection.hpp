#ifndef __DB_Oracle_OracleConnectionHPP__
#define __DB_Oracle_OracleConnectionHPP__

#include <string>
#include <cstring>

#include "occi.h"
#include "libdb/oracle/resultset.hpp"

using namespace oracle::occi;

namespace DB_Oracle {

class OracleConnection {
public:
	//! Создание соединения с базой данных.
	/*!
	  Единственный способ создать соединение.
	 */
	static OracleConnection * createConnection(std::string user, std::string password, std::string url);

	//! Закрытие соединения с базой данных.
	/*!
	  Закрывает соединение и правильно высвобождает память.
	 */
	static void terminateConnection(OracleConnection * connection);

	//! Запрос, не подразумевающий получения результата.
	void executeUpdate(std::string query);

	//! Запрос, подразумевающий результат.
	/*!
	  Возвращается объект DB_Oracle::ResultSet, и вся логика перенесена в этот класс для человеческого высвобождения памяти.
	 */
	DB_Oracle::ResultSet * executeQuery(std::string query);

private:
	//! Конструктор без параметров.
	/*!
	  Скрыт для того, чтобы соединение можно было бы получить только с помощью метода OracleConnection::createConnection().
	 */
	OracleConnection() {
		m_environment = NULL;
		m_connection = NULL;
	}		
	
	Environment * m_environment;	//!< - среда управления памятью и ресурсами для объектов OCCI.
	Connection  * m_connection;	//!< - соединение с базой данных.
};

} // namespace DB_Oracle

#endif //__DB_Oracle_OracleConnectionHPP__


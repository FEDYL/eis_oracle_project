#ifndef __DB_Oracle_ConnectionHPP__
#define __DB_Oracle_ConnectionHPP__

#include <string>
#include <list>

#include "libdb/oracle/internalconnection.hpp"
#include "libdb/common/connection.hpp"

namespace DB_Oracle {

struct InternalLogConnectionFactory : public DB::InternalConnectionFactory {
	InternalLogConnectionFactory(const char * host, const char * user, const char * pass, const char * dbname) :
		InternalConnectionFactory(host, user, pass, dbname) { }
	
	DB::InternalConnection * create() const {
		return new InternalLogConnection(host, user, pass, dbname);
	}
};

//! Соединение с базой данных с записью в лог.
/*!
  Соединение с базой данных, реализующее интерфейс DB::Connection, которое модифицирующие запросы будет записывать в лог для бэкапа.
 */
class LogConnection : public DB::Connection {
public:
	//! Конструктор.
	/*!
	  \param host - наименование хоста.
	  \param user - имя полбьзователя.
	  \param password - пароль.
	  \param dbname - наименование базы.
	  Конструирует объект используя свои аргументы.
	 */
	LogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
					DB::Connection(InternalLogConnectionFactory(host, user, pass, dbname)) { }

	//! Деструктор.
	virtual ~LogConnection() { }
};

struct InternalNoLogConnectionFactory : public DB::InternalConnectionFactory {
	InternalNoLogConnectionFactory(const char * host, const char * user, const char * pass, const char * dbname) :
		InternalConnectionFactory(host, user, pass, dbname) { }

	DB::InternalConnection * create() const {
		return new InternalNoLogConnection(host, user, pass, dbname);
	}
};

//! Соединение с базой данных без записи в лог.
/*!
  Соединение с базой данных реализующее интерфейс DB::Connection, которое не записывает запросы в лог для бэкапа.
 */
class NoLogConnection : public DB::Connection {
public:
	//! Конструктор.
	/*!
	  \param host - наименование хоста.
	  \param user - имя полбьзователя.
	  \param password - пароль.
	  \param dbname - наименование базы.
	  Конструирует объект используя свои аргументы.
	 */
	NoLogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
				DB::Connection(InternalNoLogConnectionFactory(host, user, pass, dbname)) { }
	//! Деструктор.
	virtual ~NoLogConnection() { }
};

} // namespace DB_Oracle

#endif //__DB_Oracle_ConnectionHPP__


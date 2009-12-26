#ifndef __DB_Oracle_intConnectionHPP__
#define __DB_Oracle_intConnectionHPP__

#include <list>
#include <string>

#include "libdb/oracle/oracleconnection.hpp"
#include "libdb/common/connection.hpp"
#include "libdb/common/resultset.hpp"

namespace DB_Oracle {

//! Внутреннее соединение с DBMS Oracle.
/*!
  Класс представляет собой реализацию класса DB::InternalConnection для DBMS Oracle.
 */
class InternalConnection : public DB::InternalConnection {
public:
	//! Конструктор.
	/*!
	  \param host - наименование хоста.
	  \param user - имя полбьзователя.
	  \param password - пароль.
	  \param dbname - наименование базы.
	  Создает объект и открывает соединение с DBMS Oracle.
	 */
	InternalConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL);
	//! Деструктор.
	/*!
	  Закрывает соединение с DBMS Oracle.
	 */
	virtual ~InternalConnection();

	//! Реализация метода DB::InternalConnection::start().
	/*!
	  Пока ничего не делает.
	 */
	void start() { }
	//! Реализация метода DB::InternalConnection::stop().
	/*!
	  Тоже ничего не делает. OCCI все делает сам.
	 */
	void stop() { }

	//! Реализация метода DB::InternalConnection::addSpecificToCreateTableScript().
	std::string addSpecificToCreateTableScript() { return ""; }
	//! Реализация метода DB::InternalConnection::getTableStructureFromDB().
	void getTableStructureFromDB(DB::Table * table);
	//! Реализация метода DB::InternalConnection::createDataBase().
	void createDataBase(const std::string & name);
	//! Реализация метода DB::InternalConnection::dropDataBase().
	void dropDataBase(const std::string & name);
	//! Реализация метода DB::InternalConnection::useDataBase().
	void useDataBase(const std::string & name);
	//! Реализация метода DB::InternalConnection::execConstSQL().
	void execConstSQL(const DB::Query & query);
	//! Реализация метода DB::InternalConnection::createTable().
	DB::Sequence * createSequence(const std::string & name, const unsigned int initial_value = 0);

protected:
	DB_Oracle::OracleConnection * m_orclConn;	//!< - объект, инкапсулирующий работу OCCI.

	//! Реализация метода DB::InternalConnection::executeScript().
	void executeScript(const std::string & script);

	//! Индекс базы данных.
	/*!
	  Внутренний класс, описывающий собой идекс для данной таблицы. Необходим для промежуточного хранения информации об индексе
	  на этапе чтения структуры таблицы из DBMS.
	 */
	class Index {
	public:
		//! Конструктор.
		/*!
		  \param name - наименование индекса.
		  Конструируeт объект, сохраняя наименование индекса (name).
		 */
		Index(const std::string & sname) : name(sname) { }
		std::string name;	//!< - наименование индекса.
		
		typedef std::list <std::string> FieldDefs;
		FieldDefs fields;	//!< - определения полей, входящих в индекс.
	};

	typedef std::list <Index> Indexes;

	//! Внешний ключ.
	/*!
	  Внутенний класс, описывающий собой ключ данной таблицы на другую. Необходим для промежуточного хранений информации о внешнем ключе
	  на этапе чтения структуры таблицы из DBMS.
	 */
	class ForeignKey {
	public:
		//! Конструктор.
		/*!
		  \param name - наименование ключа.
		  Конструирует объект, сохраняя наименование индекса (name).
		 */
		ForeignKey(const std::string & sname) : name(sname) { }
		std::string name;	//!< - наименование ключа.
		std::string index_name;	//!< - наименование индекса, соответствующего таблице.
		std::string table_name;	//!< - наименование таблицы, на которую ссылается ключ.
		
		//! Ссылка.
		/*!
		  Класс описывающий ссылку таблицы (поле данной таблицы ссылается на поле другой).
		 */
		class Reference {
		public:
			//! Конструктор.
			Reference() { }
			std::string field;	//!< - наименование поля таблицы, в которой есть внешний ключ.
			std::string ref_field;	//!< - наименование поля таблицы, на которую ссылается внешний ключ.
		};

		typedef std::list <Reference> References;
		References references;//!< - Список ссылок ключа.

		//! Сответствует индексу.
		/*!
		  \param index - индекс, который должен соответствовать ключу.
		  Метод утанавливает соответствие внешнего ключа индексу, ибо ключу должен обязательно соответствовать индекс.
		 */
		bool correspondIndex(Index & index);
	};

	typedef std::list <ForeignKey> ForeignKeys;
};


//! Внутреннее соединение с записью в лог.
/*!
  Внутреннее соединение с DBMS, которое записывает модифицирующие запросы в лог для бэкапа.
 */
class InternalLogConnection : public InternalConnection {
public:
	//! Конструктор.
	/*!
	  \param host - наименование хоста.
	  \param user - имя полбьзователя.
	  \param password - пароль.
	  \param dbname - наименование базы.
	  Конструирует объект используя свои аргументы.
	 */
	InternalLogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
					InternalConnection(host, user, pass, dbname) { }
	//! Деструктор.
	virtual ~InternalLogConnection() { }

	//! Выполнить SQL-запрос.
	/*!
	  \param query - запрос.
	  Выполняет SQL-запрос query, и если он модифицирующий, записывает его в лог.
	 */
	DB::ResultSet * execSQL(const DB::Query & query);
};

//! Внутреннее соединение без записи в лог.
/*!
  Внутренне соединение с DBMS, которое не записывает модифицирущие запросы в лог для быкапа, то есть запросы проходящие через это
  соединение бэкапиться не будут.
 */
class InternalNoLogConnection : public InternalConnection {
public:
	//! Конструктор.
	/*!
	  \param host - наименование хоста.
	  \param user - имя полбьзователя.
	  \param password - пароль.
	  \param dbname - наименование базы.
	  Конструирует объект используя свои аргументы.
	 */
	InternalNoLogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
					InternalConnection(host, user, pass, dbname) { }
	//! Деструктор.
	virtual ~InternalNoLogConnection() { }
	//! Выполнить SQL-запрос.
	/*!
	  \parаm query - запрос.
	  Выполняет SQL-запрос query.
	 */
	DB::ResultSet * execSQL(const DB::Query & query);
};

} // namespace DB_Oracle

#endif //__DB_Oracle_intConnectionHPP__

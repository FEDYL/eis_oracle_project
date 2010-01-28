#ifndef __DB_Oracle_intConnectionHPP__
#define __DB_Oracle_intConnectionHPP__

#include <list>
#include <string>

#include "libdb/oracle/oracleconnection.hpp"
#include "libdb/common/connection.hpp"
#include "libdb/common/resultset.hpp"

namespace DB_Oracle {

//! ���������� ���������� � DBMS Oracle.
/*!
  ����� ������������ ����� ���������� ������ DB::InternalConnection ��� DBMS Oracle.
 */
class InternalConnection : public DB::InternalConnection {
public:
	//! �����������.
	/*!
	  \param host - ������������ �����.
	  \param user - ��� �������������.
	  \param password - ������.
	  \param dbname - ������������ ����.
	  ������� ������ � ��������� ���������� � DBMS Oracle.
	 */
	InternalConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL);
	//! ����������.
	/*!
	  ��������� ���������� � DBMS Oracle.
	 */
	virtual ~InternalConnection();

	//! ���������� ������ DB::InternalConnection::start().
	/*!
	  ���� ������ �� ������.
	 */
	void start() { }
	//! ���������� ������ DB::InternalConnection::stop().
	/*!
	  ���� ������ �� ������. OCCI ��� ������ ���.
	 */
	void stop() { }

	//! ���������� ������ DB::InternalConnection::addSpecificToCreateTableScript().
	std::string addSpecificToCreateTableScript() { return ""; }
	//! ���������� ������ DB::InternalConnection::getTableStructureFromDB().
	void getTableStructureFromDB(DB::Table * table);
	//! ���������� ������ DB::InternalConnection::createDataBase().
	void createDataBase(const std::string & name);
	//! ���������� ������ DB::InternalConnection::dropDataBase().
	void dropDataBase(const std::string & name);
	//! ���������� ������ DB::InternalConnection::useDataBase().
	void useDataBase(const std::string & name);
	//! ���������� ������ DB::InternalConnection::execConstSQL().
	void execConstSQL(const DB::Query & query);
	//! ���������� ������ DB::InternalConnection::createTable().
	DB::Sequence * createSequence(const std::string & name, const unsigned int initial_value = 0);

protected:
	DB_Oracle::OracleConnection * m_orclConn;	//!< - ������, ��������������� ������ OCCI.

	//! ���������� ������ DB::InternalConnection::executeScript().
	void executeScript(const std::string & script);

	//! ������ ���� ������.
	/*!
	  ���������� �����, ����������� ����� ����� ��� ������ �������. ��������� ��� �������������� �������� ���������� �� �������
	  �� ����� ������ ��������� ������� �� DBMS.
	 */
	class Index {
	public:
		//! �����������.
		/*!
		  \param name - ������������ �������.
		  ����������e� ������, �������� ������������ ������� (name).
		 */
		Index(const std::string & sname) : name(sname) { }
		std::string name;	//!< - ������������ �������.
		
		typedef std::list <std::string> FieldDefs;
		FieldDefs fields;	//!< - ����������� �����, �������� � ������.
	};

	typedef std::list <Index> Indexes;

	//! ������� ����.
	/*!
	  ��������� �����, ����������� ����� ���� ������ ������� �� ������. ��������� ��� �������������� �������� ���������� � ������� �����
	  �� ����� ������ ��������� ������� �� DBMS.
	 */
	class ForeignKey {
	public:
		//! �����������.
		/*!
		  \param name - ������������ �����.
		  ������������ ������, �������� ������������ ������� (name).
		 */
		ForeignKey(const std::string & sname) : name(sname) { }
		std::string name;	//!< - ������������ �����.
		std::string index_name;	//!< - ������������ �������, ���������������� �������.
		std::string table_name;	//!< - ������������ �������, �� ������� ��������� ����.
		
		//! ������.
		/*!
		  ����� ����������� ������ ������� (���� ������ ������� ��������� �� ���� ������).
		 */
		class Reference {
		public:
			//! �����������.
			Reference() { }
			std::string field;	//!< - ������������ ���� �������, � ������� ���� ������� ����.
			std::string ref_field;	//!< - ������������ ���� �������, �� ������� ��������� ������� ����.
		};

		typedef std::list <Reference> References;
		References references;//!< - ������ ������ �����.

		//! ������������ �������.
		/*!
		  \param index - ������, ������� ������ ��������������� �����.
		  ����� ������������ ������������ �������� ����� �������, ��� ����� ������ ����������� ��������������� ������.
		 */
		bool correspondIndex(Index & index);
	};

	typedef std::list <ForeignKey> ForeignKeys;
};


//! ���������� ���������� � ������� � ���.
/*!
  ���������� ���������� � DBMS, ������� ���������� �������������� ������� � ��� ��� ������.
 */
class InternalLogConnection : public InternalConnection {
public:
	//! �����������.
	/*!
	  \param host - ������������ �����.
	  \param user - ��� �������������.
	  \param password - ������.
	  \param dbname - ������������ ����.
	  ������������ ������ ��������� ���� ���������.
	 */
	InternalLogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
					InternalConnection(host, user, pass, dbname) { }
	//! ����������.
	virtual ~InternalLogConnection() { }

	//! ��������� SQL-������.
	/*!
	  \param query - ������.
	  ��������� SQL-������ query, � ���� �� ��������������, ���������� ��� � ���.
	 */
	DB::ResultSet * execSQL(const DB::Query & query);
};

//! ���������� ���������� ��� ������ � ���.
/*!
  ��������� ���������� � DBMS, ������� �� ���������� ������������� ������� � ��� ��� ������, �� ���� ������� ���������� ����� ���
  ���������� ���������� �� �����.
 */
class InternalNoLogConnection : public InternalConnection {
public:
	//! �����������.
	/*!
	  \param host - ������������ �����.
	  \param user - ��� �������������.
	  \param password - ������.
	  \param dbname - ������������ ����.
	  ������������ ������ ��������� ���� ���������.
	 */
	InternalNoLogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
					InternalConnection(host, user, pass, dbname) { }
	//! ����������.
	virtual ~InternalNoLogConnection() { }
	//! ��������� SQL-������.
	/*!
	  \par�m query - ������.
	  ��������� SQL-������ query.
	 */
	DB::ResultSet * execSQL(const DB::Query & query);
};

} // namespace DB_Oracle

#endif //__DB_Oracle_intConnectionHPP__

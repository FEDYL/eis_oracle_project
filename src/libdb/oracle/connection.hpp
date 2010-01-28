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

//! ���������� � ����� ������ � ������� � ���.
/*!
  ���������� � ����� ������, ����������� ��������� DB::Connection, ������� �������������� ������� ����� ���������� � ��� ��� ������.
 */
class LogConnection : public DB::Connection {
public:
	//! �����������.
	/*!
	  \param host - ������������ �����.
	  \param user - ��� �������������.
	  \param password - ������.
	  \param dbname - ������������ ����.
	  ������������ ������ ��������� ���� ���������.
	 */
	LogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
					DB::Connection(InternalLogConnectionFactory(host, user, pass, dbname)) { }

	//! ����������.
	virtual ~LogConnection() { }
};

struct InternalNoLogConnectionFactory : public DB::InternalConnectionFactory {
	InternalNoLogConnectionFactory(const char * host, const char * user, const char * pass, const char * dbname) :
		InternalConnectionFactory(host, user, pass, dbname) { }

	DB::InternalConnection * create() const {
		return new InternalNoLogConnection(host, user, pass, dbname);
	}
};

//! ���������� � ����� ������ ��� ������ � ���.
/*!
  ���������� � ����� ������ ����������� ��������� DB::Connection, ������� �� ���������� ������� � ��� ��� ������.
 */
class NoLogConnection : public DB::Connection {
public:
	//! �����������.
	/*!
	  \param host - ������������ �����.
	  \param user - ��� �������������.
	  \param password - ������.
	  \param dbname - ������������ ����.
	  ������������ ������ ��������� ���� ���������.
	 */
	NoLogConnection(const char * host, const char * user, const char * pass, const char * dbname = NULL) :
				DB::Connection(InternalNoLogConnectionFactory(host, user, pass, dbname)) { }
	//! ����������.
	virtual ~NoLogConnection() { }
};

} // namespace DB_Oracle

#endif //__DB_Oracle_ConnectionHPP__


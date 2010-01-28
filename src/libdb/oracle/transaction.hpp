#ifndef __DB_Oracle_TransactionHPP__
#define __DB_Oracle_TransactionHPP__

#include "libdb/common/connection.hpp"

namespace DB_Oracle {

//! ����������.
/*!
  ���������� ����������, ��������� ������� ����� ���� ������ ������ ���������,
  ������������� ������������ ���� DB_Oracle.
 */
class Transaction {
public:
	//! �����������.
	/*!
	  ������� ���������� � �������� �� (��������� SQL-�������� BEGIN,
	  � ������, ���� �������� � ������ ���������� � ����� �� ������ ����������.
	  \param connection - ���������� � ����� ������.
	 */
	Transaction(DB::InternalConnection * connection);
	//! ����������.
	/*!
	  ����� ������������ ������� ��������� �������� SQL-�������� ROLLBACK,
	  � ������, ���� �������� �� ������ ����������, � �� ��� ������
	  ����� commit.
	 */
	~Transaction();
	
	//! ������������� ���������.
	/*!
	  ��������� ��������� ����������� � ���� � ������ ���� ����������
	  (������� SQL-�������� COMMIT), � ������, ���� �������� �� ������
	  ����������.
	 */
	void commit();

private:
	DB::InternalConnection * m_connection;	//!< - ��������� �� ���������� ���������� � �����.
	MT::SingleLock m_lock;
	bool m_is_commited;			//!< - ���� ����������������� ���������.
};

}  // namespace DB_Oracle

#endif //__DB_Oracle_TransactionHPP__

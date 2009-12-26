#ifndef __DB_Oracle_SequenceHPP__
#define __DB_Oracle_SequenceHPP__

#include "libdb/common/sequence.hpp"

namespace DB_Oracle {

//! ��������� ���������������� ��������.
/*!
  ���������� ���������� DB::Sequence.
 */
class Sequence : public DB::Sequence {
friend class Factory;
friend class InternalConnection;
private:
	//! �����������
	/*!
	  \param name - ������������ ����������.
	  ������� ������ � ������������� name. 
	 */
	Sequence(const std::string & name, const unsigned int initial_value = 0);
	//! �����������
	/*!
	  \param name - ������������ ����������.
	  \param connection - ���������� � ����� �������.
	  ������� ������ � ������������� name. 
	 */
	Sequence(DB::InternalConnection * connection, const std::string & name, const unsigned int initial_value = 0);

public:
	//! ����������.
	virtual ~Sequence() {};

	//! ���������� ������ DB::Sequence::getNextVal().
	unsigned long long getNextVal(unsigned int increment = 1);

	//! ���������� ������ DB::Sequence::getValue().
	unsigned long long getValue();

	//! ���������� ������ DB::InternalParameter::str().
	std::string str() const;

private:
	unsigned long long m_value;	//!< - ������� ��������.

	//! ���������� ������ DB::DBMSObject::create().
	void create(DB::InternalConnection * connection, bool if_not_exists = false);
	
	//! ���������� ������ DB::Sequence::drop().
	void drop(bool if_exists = false);
	
	//! ���������� ������ DB::Sequence::drop().
	void drop(DB::InternalConnection * connection, bool if_exists = false);
};

} // namespace DB_Oracle

#endif //__DB_Oracle_SequenceHPP__

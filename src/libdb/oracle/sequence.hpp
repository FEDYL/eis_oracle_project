#ifndef __DB_Oracle_SequenceHPP__
#define __DB_Oracle_SequenceHPP__

#include "libdb/common/sequence.hpp"

namespace DB_Oracle {

//! Генератор последовательных значений.
/*!
  Реализация интерфейса DB::Sequence.
 */
class Sequence : public DB::Sequence {
friend class Factory;
friend class InternalConnection;
private:
	//! Конструктор
	/*!
	  \param name - наименование генератора.
	  Создает объект с наименованием name. 
	 */
	Sequence(const std::string & name, const unsigned int initial_value = 0);
	//! Конструктор
	/*!
	  \param name - наименование генератора.
	  \param connection - соединение с базой вестимо.
	  Создает объект с наименованием name. 
	 */
	Sequence(DB::InternalConnection * connection, const std::string & name, const unsigned int initial_value = 0);

public:
	//! Деструктор.
	virtual ~Sequence() {};

	//! Реализация метода DB::Sequence::getNextVal().
	unsigned long long getNextVal(unsigned int increment = 1);

	//! Реализация метода DB::Sequence::getValue().
	unsigned long long getValue();

	//! Реализация метода DB::InternalParameter::str().
	std::string str() const;

private:
	unsigned long long m_value;	//!< - текущее значение.

	//! Реализация метода DB::DBMSObject::create().
	void create(DB::InternalConnection * connection, bool if_not_exists = false);
	
	//! Реализация метода DB::Sequence::drop().
	void drop(bool if_exists = false);
	
	//! Реализация метода DB::Sequence::drop().
	void drop(DB::InternalConnection * connection, bool if_exists = false);
};

} // namespace DB_Oracle

#endif //__DB_Oracle_SequenceHPP__

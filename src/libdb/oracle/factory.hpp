#ifndef __DB_Oracle_FactoryHPP__
#define __DB_Oracle_FactoryHPP__

#include "libdb/common/factory.hpp"

namespace DB_Oracle {

//! ���������� ���������� DB::Factory.
class Factory : public DB::Factory {
public:
	//! ������� ��������� �������.
	static void createInstance();

	//! ���������� ������ DB::Factory::createLogConnection().
	virtual DB::Connection * createLogConnection(const char * host, const char * user, const char * pass, const char * dbname = "");

	//! ���������� ������ DB::Factory::createNoLogConnection().
	virtual DB::Connection * createNoLogConnection(const char * host, const char * user, const char * pass, const char * dbname = "");

	//! ���������� ������ DB::Factory::createSequence().
	/*!
	  ������� ����������, ��� ����� �� ��������������. Sequence ��� �������� � ����������� ����������� ������ �� �����.
	 */
	virtual DB::Sequence * createSequence(const std::string & name, unsigned int initial_value = 0);

private:
	//! ���������� �����������.
	/*!
	  ������� � private-������, � ����� ���������� � ���������� ���������� ������ ���� � ���� ��������.
	 */
	Factory() { }
};
	
} // namespace DB_Oracle

#endif //__DB_Oracle_FactoryHPP__

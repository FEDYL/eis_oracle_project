#ifndef __DB_Oracle_FactoryHPP__
#define __DB_Oracle_FactoryHPP__

#include "libdb/common/factory.hpp"

namespace DB_Oracle {

//! Реализайия интерфейса DB::Factory.
class Factory : public DB::Factory {
public:
	//! Создает экземпляр фабрики.
	static void createInstance();

	//! Реализация метода DB::Factory::createLogConnection().
	virtual DB::Connection * createLogConnection(const char * host, const char * user, const char * pass, const char * dbname = "");

	//! Реализация метода DB::Factory::createNoLogConnection().
	virtual DB::Connection * createNoLogConnection(const char * host, const char * user, const char * pass, const char * dbname = "");

	//! Реализация метода DB::Factory::createSequence().
	virtual DB::Sequence * createSequence(const std::string & name, unsigned int initial_value = 0);

private:
	//! Дефолтовый конструктор.
	/*!
	  Помещен в private-секцию, с целью запрещения в объявления переменных такого типа в коде сервисов.
	 */
	Factory() { }
};
	
} // namespace DB_Oracle

#endif //__DB_Oracle_FactoryHPP__

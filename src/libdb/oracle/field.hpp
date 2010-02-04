#ifndef __DB_Oracle_FieldHPP__
#define __DB_Oracle_FieldHPP__

#include <string>
#include <iostream>
#include <sstream>

#include "occi.h"

#include "libdb/common/field.hpp"

namespace DB_Oracle {

//! Информация о поле.
/*!
  Содержит всю информацию о поле кроме непосредственно данных.
 */
struct ORACLE_FIELD {
	//! Простенький конструктор.
	ORACLE_FIELD() : length(0), precision(0), scale(0), type(0) { }

	//! Перечисление типов полей. Значения взяты из документации OCCI.
	static const unsigned int FIELD_TYPE_NUMBER = 0x2;
	static const unsigned int FIELD_TYPE_TIMESTAMP = 0xBB;
	static const unsigned int FIELD_TYPE_STRING = 0x1;
	static const unsigned int FIELD_TYPE_BLOB = 0x71;

	std::string name;		//!< - имя поля.
	std::string table;	  	//!< - имя таблицы, из которой было выбрано это поле.
	std::string def;		//!< - значение поля по умолчанию.
	unsigned int length;		//!< - размер поля.
	unsigned int precision;		//!< - разрядность. Только для типа FIELD_TYPE_NUMBER.
	unsigned int scale;		//!< - знаки после запятой. Только для типа FIELD_TYPE_NUMBER.
	unsigned int type;		//!< - тип поля.
};

//! Поле базы данных.
/*!
  Поле базы данных в результате запроса. Не содержит данных о содержимом поля. 
  Только тип, наименование и таблица. Содержимое реализуется в наследниках.
 */
class Field : public DB::Field {
public:
	//! Конструктор.
	/*!
	  \param field - структура со всеми данными о поле.
	  Конструирует объект типа field.type с наименованием field.name принадлежащий таблице field.table.
	 */
	Field(const ORACLE_FIELD &field) : DB::Field(FT_UNKNOWN), _name(field.name), _table(field.table) { }

	//! Конструктор копирования.
	Field(const Field &src) : DB::Field(src), _name(src._name), _table(src._table) { }

	//! Оператор присваивания.
	Field& operator=(const Field &src) {
		_name = src._name;
		_table = src._table;
		return *this;
	}
	
	//! Пустая реализация метода DB::Field::setData()
	/*!
	  Пустая она, так как объекты DB_Oracle::Field не создаются из const char *
	 */
	void setData(size_t, const char *) { 
		throw DB::XDBError("Unsupported operation: Field::setData()");
	}
	//! Пустая реализация метода DB::Field::c_str()
	/*!
	  Пустая она, так как объекты DB_Oracle::Field не создаются из const char *
	 */
	const char * c_str() const { 
		throw DB::XDBError("Unsupported operation: Field::c_str()"); 
		return NULL; 
	}

	//! Акцептор к наименованию поля.
	const std::string&   name() const  { return _name; }
	//! Акцептор к наименованию таблицы.
	const std::string&   table() const { return _table; }

	//! Устанавливает флаг _null.
	void setNull() {
		isNull(true);
	}

private:
	std::string _name;	//!< - наименование поля.
	std::string _table;	//!< - наименование таблицы.
};



//! Реализация поля базы данных для оракловского типа Number.
/*!
  Этот оракловский тип включает в себя все целые поля, float, double, тип bool и тип numeric. 
 */
class NumberField : public Field {
public:
	//! Конструктор.
	/*!
	  \param field - структура со всеми данными о поле.
	  \param data - данные.
	  Конструирует объект на основе данных своих аргументов.
	 */
	NumberField(const ORACLE_FIELD &field, const oracle::occi::Number &data) : Field(field), _prec(field.precision), _scale(field.scale), 
							_data(data), _numeric_data(field.precision - field.scale, field.scale, asDouble()) { }

	//! Конструктор создающий поле c пустыми данными.
	/*!
	  \param field - структура со всеми данными о поле.
	 */
	NumberField(const ORACLE_FIELD &field) : Field(field), _prec(field.precision), _scale(field.scale), 
							_data(0), _numeric_data(field.precision - field.scale, field.scale, 0) {
		setNull();
	}

	//! Конструктор копирования.
	NumberField(const NumberField &src) : Field(src), _data(src._data), _numeric_data(src._numeric_data), _prec(src._prec), _scale(src._scale) { }

	//! Деструктор.
	~NumberField() { }

	//! Реализация методов DB::Field.
	int asLong() const;
	unsigned int asULong() const;
	short asShort() const;
	unsigned short asUShort() const;
	long long asLongLong() const;
	unsigned long long asULongLong() const;
	bool asBool() const;
	float asFloat() const;
	double asDouble() const;
	const Numeric & asNumeric() const;

	void assignValue(int &value) const  { value = asLong(); }
	void assignValue(unsigned int &value) const  { value = asULong(); }
	void assignValue(short &value) const  { value = asShort(); }
	void assignValue(unsigned short &value) const { value = asUShort(); }
	void assignValue(long long &value) const { value = asLongLong(); }
	void assignValue(unsigned long long &value) const { value = asULongLong(); }
	void assignValue(float &value) const { value = asFloat(); }
	void assignValue(double &value) const { value = asDouble(); }
	void assignValue(bool &value) const { value = asBool(); }
	void assignValue(Numeric &value) const { value = asNumeric(); }
	
	//! Клонировать.
	/*!
	  Создает подобного себе.
	 */
	virtual	DB::Field * _clone() const { 
		return new NumberField(*this);
	}

	//! Присвоить данные аргументу типа CORBA::Any.
	/*!
	  Присваисвает данные аргументу value.
	  \param value - ссылка на переменную, которой осуществляется присваивание.
	 */
	void assignAny(CORBA::Any &value) const;
	
	//! Реализация метода DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! Устанавливает новые значения поля.
	/*!
	 Создан на замену неподдерживаемому DB::Field::setData(size_t, const char *).
	 */
	void setData(const oracle::occi::Number &data);

private:
	oracle::occi::Number _data;	//!< - данные.
	Numeric	_numeric_data;		//!< - данные в другом формате.
	unsigned int _prec;		//!< - разрядность.
	unsigned int _scale;		//!< - знаки после запятой.
};



//! Реализация поля базы данных для оракловского типа varchar2.
/*!
  Строка и в Африке строка.
 */
class StringField : public Field {
public:
	//! Конструктор.
	/*!
	  \param field - структура со всеми данными о поле.
	  \param data - данные.
	  Конструирует объект на основе данных своих аргументов.
	 */
	StringField(const ORACLE_FIELD &field, const std::string &data) : Field(field) {
		setData(data);
	}

	//! Конструктор создающий поле c пустыми данными.
	/*!
	  \param field - структура со всеми данными о поле.
	 */
	StringField(const ORACLE_FIELD &field) : Field(field) {
		setNull();
	}


	//! Конструктор копирования.
	StringField(const StringField &src) : Field(src) { _data.assign(src._data); }

	//! Деструктор.
	~StringField() { }

	//! Реализация методов DB::Field.
	const std::string & asString() const;
	char asChar() const;

	void assignValue(std::string &value) const { value = asString(); }
	void assignValue(char &value) const { value = asChar(); }
	
	//! Клонировать.
	/*!
	  Создает подобного себе.
	 */
	virtual	DB::Field * _clone() const { 
		return new StringField(*this); 
	}

	//! Присвоить данные аргументу типа CORBA::Any.
	/*!
	  Присваисвает данные аргументу value.
	  \param value - ссылка на переменную, которой осуществляется присваивание.
	 */
	void assignAny(CORBA::Any &value) const;

	//! Реализация метода DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! Устанавливает новое значение поля.
	/*!
	 Создан на замену неподдерживаемому DB::Field::setData(size_t, const char *).
	 */
	void setData(const std::string &data) {
		isNull(false);
		_data.assign(data);
	}

private:
	std::string _data;	//!< - данные.
};



//! Реализация поля базы данных для оракловского типа Timestamp.
/*!
  Поле, значения которого представляют собой дату и/или время.
 */
class TimestampField : public Field {
public:
	//! Конструктор.
	/*!
	  \param field - структура со всеми данными о поле.
	  \param data - данные.
	  Конструирует объект на основе данных своих аргументов.
	 */
	TimestampField(const ORACLE_FIELD &field, const oracle::occi::Timestamp &data) : Field(field) {
		setData(data);
	}

	//! Конструктор создающий поле c пустыми данными.
	/*!
	  \param field - структура со всеми данными о поле.
	 */
	TimestampField(const ORACLE_FIELD &field) : Field(field) {
		setNull();
	}

	//! Конструктор копирования.
	TimestampField(const TimestampField &src) : Field(src), _time(src._time), _date(src._date), _datetime(src._datetime) { }

	//! Деструктор.
	~TimestampField() { }
	
	//! Реализация методов DB::Field.
	const eis_date::datetime & asDateTime() const;
	const eis_date::time & asTime() const;	
	const eis_date::date & asDate() const;

	void assignValue(eis_date::datetime &value) const { value = asDateTime(); }
	void assignValue(eis_date::time &value) const { value = asTime(); }
	void assignValue(eis_date::date &value) const { value = asDate(); }

	//! Клонировать.
	/*!
	  Создает подобного себе.
	 */
	virtual DB::Field * _clone() const { 
		return new TimestampField(*this);
	}

	//! Присвоить данные аргументу типа CORBA::Any.
	/*!
	  Присваисвает данные аргументу value.
	  \param value - ссылка на переменную, которой осущесвляется присваивание.
	 */
	void assignAny(CORBA::Any &value) const;

	//! Реализация метода DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! Устанавливает новые значения поля.
	/*!
	 Создан на замену неподдерживаемому DB::Field::setData(size_t, const char *).
	 */
	void setData(const oracle::occi::Timestamp &data);

private:
	eis_date::datetime	_datetime;	//!< - данные в формате eis_date::datetime.
	eis_date::time		_time;		//!< - данные в формате eis_date::time.
	eis_date::date		_date;		//!< - данные в формате eis_date::date.
};



//! Поле типа Blob.
/*!
  Поле, которое содержит бинарные данные.
 */
class BlobField : public Field {
public:
	//! Конструктор.
	/*!
	  \param field - структура со всеми данными о поле.
	  \param data - данные.
	  Конструирует объект на основе данных своих аргументов.
	 */
	BlobField(const ORACLE_FIELD &field, const oracle::occi::Blob &data) : Field(field) {
		setData(data);
	}

	//! Конструктор создающий поле c пустыми данными.
	/*!
	  \param field - структура со всеми данными о поле.
	 */
	BlobField(const ORACLE_FIELD &field) : Field(field) {
		setNull();
	}
	
	//! Конструктор копирования.
	BlobField(const BlobField &src) : Field(src), _data(src._data) { }

	//! Деструктор.
	~BlobField() { }
	
	//! Реализация методов DB::Field.
	const DB::Blob & asBlob() const;

	void assignValue(DB::Blob &value) const { value = asBlob(); }
	
	//! Клонировать.
	/*!
	  Создает подобного себе.
	 */
	virtual	DB::Field * _clone() const { 
		return new BlobField(*this);
	}

	//! Присвоить данные аргументу типа CORBA::Any.
	/*!
	  Присваисвает данные аргументу value.
	  \param value - ссылка на переменную, которой осущесвляется присваивание.
	 */
	void assignAny(CORBA::Any &value) const;
	
	//! Реализация метода DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! Устанавливает новое значение поля.
	/*!
	 Создан на замену неподдерживаемому DB::Field::setData(size_t, const char *).
	 */
	void setData(const oracle::occi::Blob &data);

private:
	DB::Blob _data; //!< - данные.
};

} // namespace DB_Oracle

#endif //__DB_Oracle_FieldHPP__

#ifndef __DB_Oracle_FieldHPP__
#define __DB_Oracle_FieldHPP__

#include <string>
#include <iostream>
#include <sstream>

#include "occi.h"

#include "libdb/common/field.hpp"

namespace DB_Oracle {

//! ���������� � ����.
/*!
  �������� ��� ���������� � ���� ����� ��������������� ������.
 */
struct ORACLE_FIELD {
	//! ����������� �����������.
	ORACLE_FIELD() : length(0), precision(0), scale(0), type(0) { }

	//! ������������ ����� �����. �������� ����� �� ������������ OCCI.
	static const unsigned int FIELD_TYPE_NUMBER = 0x2;
	static const unsigned int FIELD_TYPE_TIMESTAMP = 0xBB;
	static const unsigned int FIELD_TYPE_STRING = 0x1;
	static const unsigned int FIELD_TYPE_BLOB = 0x71;

	std::string name;		//!< - ��� ����.
	std::string table;	  	//!< - ��� �������, �� ������� ���� ������� ��� ����.
	std::string def;		//!< - �������� ���� �� ���������.
	unsigned int length;		//!< - ������ ����.
	unsigned int precision;		//!< - �����������. ������ ��� ���� FIELD_TYPE_NUMBER.
	unsigned int scale;		//!< - ����� ����� �������. ������ ��� ���� FIELD_TYPE_NUMBER.
	unsigned int type;		//!< - ��� ����.
};

//! ���� ���� ������.
/*!
  ���� ���� ������ � ���������� �������. �� �������� ������ � ���������� ����. 
  ������ ���, ������������ � �������. ���������� ����������� � �����������.
 */
class Field : public DB::Field {
public:
	//! �����������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	  ������������ ������ ���� field.type � ������������� field.name ������������� ������� field.table.
	 */
	Field(const ORACLE_FIELD &field) : DB::Field(FT_UNKNOWN), _name(field.name), _table(field.table) { }

	//! ����������� �����������.
	Field(const Field &src) : DB::Field(src), _name(src._name), _table(src._table) { }

	//! �������� ������������.
	Field& operator=(const Field &src) {
		_name = src._name;
		_table = src._table;
		return *this;
	}
	
	//! ������ ���������� ������ DB::Field::setData()
	/*!
	  ������ ���, ��� ��� ������� DB_Oracle::Field �� ��������� �� const char *
	 */
	void setData(size_t, const char *) { 
		throw DB::XDBError("Unsupported operation: Field::setData()");
	}
	//! ������ ���������� ������ DB::Field::c_str()
	/*!
	  ������ ���, ��� ��� ������� DB_Oracle::Field �� ��������� �� const char *
	 */
	const char * c_str() const { 
		throw DB::XDBError("Unsupported operation: Field::c_str()"); 
		return NULL; 
	}

	//! �������� � ������������ ����.
	const std::string&   name() const  { return _name; }
	//! �������� � ������������ �������.
	const std::string&   table() const { return _table; }

	//! ������������� ���� _null.
	void setNull() {
		isNull(true);
	}

private:
	std::string _name;	//!< - ������������ ����.
	std::string _table;	//!< - ������������ �������.
};



//! ���������� ���� ���� ������ ��� ������������ ���� Number.
/*!
  ���� ����������� ��� �������� � ���� ��� ����� ����, float, double, ��� bool � ��� numeric. 
 */
class NumberField : public Field {
public:
	//! �����������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	  \param data - ������.
	  ������������ ������ �� ������ ������ ����� ����������.
	 */
	NumberField(const ORACLE_FIELD &field, const oracle::occi::Number &data) : Field(field), _prec(field.precision), _scale(field.scale), 
							_data(data), _numeric_data(field.precision - field.scale, field.scale, asDouble()) { }

	//! ����������� ��������� ���� c ������� �������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	 */
	NumberField(const ORACLE_FIELD &field) : Field(field), _prec(field.precision), _scale(field.scale), 
							_data(0), _numeric_data(field.precision - field.scale, field.scale, 0) {
		setNull();
	}

	//! ����������� �����������.
	NumberField(const NumberField &src) : Field(src), _data(src._data), _numeric_data(src._numeric_data), _prec(src._prec), _scale(src._scale) { }

	//! ����������.
	~NumberField() { }

	//! ���������� ������� DB::Field.
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
	
	//! �����������.
	/*!
	  ������� ��������� ����.
	 */
	virtual	DB::Field * _clone() const { 
		return new NumberField(*this);
	}

	//! ��������� ������ ��������� ���� CORBA::Any.
	/*!
	  ������������ ������ ��������� value.
	  \param value - ������ �� ����������, ������� �������������� ������������.
	 */
	void assignAny(CORBA::Any &value) const;
	
	//! ���������� ������ DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! ������������� ����� �������� ����.
	/*!
	 ������ �� ������ ����������������� DB::Field::setData(size_t, const char *).
	 */
	void setData(const oracle::occi::Number &data);

private:
	oracle::occi::Number _data;	//!< - ������.
	Numeric	_numeric_data;		//!< - ������ � ������ �������.
	unsigned int _prec;		//!< - �����������.
	unsigned int _scale;		//!< - ����� ����� �������.
};



//! ���������� ���� ���� ������ ��� ������������ ���� varchar2.
/*!
  ������ � � ������ ������.
 */
class StringField : public Field {
public:
	//! �����������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	  \param data - ������.
	  ������������ ������ �� ������ ������ ����� ����������.
	 */
	StringField(const ORACLE_FIELD &field, const std::string &data) : Field(field) {
		setData(data);
	}

	//! ����������� ��������� ���� c ������� �������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	 */
	StringField(const ORACLE_FIELD &field) : Field(field) {
		setNull();
	}


	//! ����������� �����������.
	StringField(const StringField &src) : Field(src) { _data.assign(src._data); }

	//! ����������.
	~StringField() { }

	//! ���������� ������� DB::Field.
	const std::string & asString() const;
	char asChar() const;

	void assignValue(std::string &value) const { value = asString(); }
	void assignValue(char &value) const { value = asChar(); }
	
	//! �����������.
	/*!
	  ������� ��������� ����.
	 */
	virtual	DB::Field * _clone() const { 
		return new StringField(*this); 
	}

	//! ��������� ������ ��������� ���� CORBA::Any.
	/*!
	  ������������ ������ ��������� value.
	  \param value - ������ �� ����������, ������� �������������� ������������.
	 */
	void assignAny(CORBA::Any &value) const;

	//! ���������� ������ DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! ������������� ����� �������� ����.
	/*!
	 ������ �� ������ ����������������� DB::Field::setData(size_t, const char *).
	 */
	void setData(const std::string &data) {
		isNull(false);
		_data.assign(data);
	}

private:
	std::string _data;	//!< - ������.
};



//! ���������� ���� ���� ������ ��� ������������ ���� Timestamp.
/*!
  ����, �������� �������� ������������ ����� ���� �/��� �����.
 */
class TimestampField : public Field {
public:
	//! �����������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	  \param data - ������.
	  ������������ ������ �� ������ ������ ����� ����������.
	 */
	TimestampField(const ORACLE_FIELD &field, const oracle::occi::Timestamp &data) : Field(field) {
		setData(data);
	}

	//! ����������� ��������� ���� c ������� �������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	 */
	TimestampField(const ORACLE_FIELD &field) : Field(field) {
		setNull();
	}

	//! ����������� �����������.
	TimestampField(const TimestampField &src) : Field(src), _time(src._time), _date(src._date), _datetime(src._datetime) { }

	//! ����������.
	~TimestampField() { }
	
	//! ���������� ������� DB::Field.
	const eis_date::datetime & asDateTime() const;
	const eis_date::time & asTime() const;	
	const eis_date::date & asDate() const;

	void assignValue(eis_date::datetime &value) const { value = asDateTime(); }
	void assignValue(eis_date::time &value) const { value = asTime(); }
	void assignValue(eis_date::date &value) const { value = asDate(); }

	//! �����������.
	/*!
	  ������� ��������� ����.
	 */
	virtual DB::Field * _clone() const { 
		return new TimestampField(*this);
	}

	//! ��������� ������ ��������� ���� CORBA::Any.
	/*!
	  ������������ ������ ��������� value.
	  \param value - ������ �� ����������, ������� ������������� ������������.
	 */
	void assignAny(CORBA::Any &value) const;

	//! ���������� ������ DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! ������������� ����� �������� ����.
	/*!
	 ������ �� ������ ����������������� DB::Field::setData(size_t, const char *).
	 */
	void setData(const oracle::occi::Timestamp &data);

private:
	eis_date::datetime	_datetime;	//!< - ������ � ������� eis_date::datetime.
	eis_date::time		_time;		//!< - ������ � ������� eis_date::time.
	eis_date::date		_date;		//!< - ������ � ������� eis_date::date.
};



//! ���� ���� Blob.
/*!
  ����, ������� �������� �������� ������.
 */
class BlobField : public Field {
public:
	//! �����������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	  \param data - ������.
	  ������������ ������ �� ������ ������ ����� ����������.
	 */
	BlobField(const ORACLE_FIELD &field, const oracle::occi::Blob &data) : Field(field) {
		setData(data);
	}

	//! ����������� ��������� ���� c ������� �������.
	/*!
	  \param field - ��������� �� ����� ������� � ����.
	 */
	BlobField(const ORACLE_FIELD &field) : Field(field) {
		setNull();
	}
	
	//! ����������� �����������.
	BlobField(const BlobField &src) : Field(src), _data(src._data) { }

	//! ����������.
	~BlobField() { }
	
	//! ���������� ������� DB::Field.
	const DB::Blob & asBlob() const;

	void assignValue(DB::Blob &value) const { value = asBlob(); }
	
	//! �����������.
	/*!
	  ������� ��������� ����.
	 */
	virtual	DB::Field * _clone() const { 
		return new BlobField(*this);
	}

	//! ��������� ������ ��������� ���� CORBA::Any.
	/*!
	  ������������ ������ ��������� value.
	  \param value - ������ �� ����������, ������� ������������� ������������.
	 */
	void assignAny(CORBA::Any &value) const;
	
	//! ���������� ������ DB::Field::put().
	std::ostream & put(std::ostream &) const;

	//! ������������� ����� �������� ����.
	/*!
	 ������ �� ������ ����������������� DB::Field::setData(size_t, const char *).
	 */
	void setData(const oracle::occi::Blob &data);

private:
	DB::Blob _data; //!< - ������.
};

} // namespace DB_Oracle

#endif //__DB_Oracle_FieldHPP__

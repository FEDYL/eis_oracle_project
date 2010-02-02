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
		throw DB::XDBError("This operation is not supported: Field::setData()");
	}
	//! ������ ���������� ������ DB::Field::c_str()
	/*!
	  ������ ���, ��� ��� ������� DB_Oracle::Field �� ��������� �� const char *
	 */
	const char * c_str() const { 
		throw DB::XDBError("This operation is not supported: Field::c_str()"); 
		return NULL; 
	}

	//! �������� � ������������ ����.
	const std::string&   name() const  { return _name; }
	//! �������� � ������������ �������.
	const std::string&   table() const { return _table; }

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
	NumberField(const ORACLE_FIELD &field, oracle::occi::Number data) : Field(field), _data(data), 
								_numeric_data(field.precision - field.scale, field.scale, (double)data) { }

	//! ����������� �����������.
	NumberField(const NumberField &src) : Field(src), _data(src._data), _numeric_data(src._numeric_data) { }

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
	virtual	DB::Field * _clone() const { return new NumberField(*this); }

	//! ��������� ������ ��������� ���� CORBA::Any.
	/*!
	  ������������ ������ ��������� value.
	  \param value - ������ �� ����������, ������� �������������� ������������.
	 */
	void assignAny(CORBA::Any &value) const;
	
	//! ���������� ������ DB::Field::put().
	std::ostream & put(std::ostream &) const;

private:
	oracle::occi::Number _data;	//!< - ������.
	Numeric	_numeric_data;		//!< - ������ � ������ �������.
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
	StringField(const ORACLE_FIELD &field, std::string data) : Field(field) {
		_data.assign(data);
	}

	//! ����������� �����������.
	StringField(const StringField &src) : Field(src) { _data.assign(src._data); }

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
	TimestampField(const ORACLE_FIELD &field, oracle::occi::Timestamp data) : Field(field), _data(data) {
		convertType();
	}

	//! ����������� �����������.
	TimestampField(const TimestampField &src) : Field(src), _data(src._data), _eis_time_data(src._eis_time_data),
								_eis_date_data(src._eis_date_data), _eis_datetime_data(src._eis_datetime_data) { }
	
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
	virtual DB::Field * _clone() const { return new TimestampField(*this); }

	//! ��������� ������ ��������� ���� CORBA::Any.
	/*!
	  ������������ ������ ��������� value.
	  \param value - ������ �� ����������, ������� ������������� ������������.
	 */
	void assignAny(CORBA::Any &value) const;

	//! ���������� ������ DB::Field::put().
	std::ostream & put(std::ostream &) const;

private:
	//! ��������� �������� ���� _data � ��������� ��� � ������� eis_date::datetime, eis_date::time � eis_date::date.
	void convertType() {
		unsigned int hour, minute, sec, sc;	// ��������� �������.
		int zone_hour, zone_minute;		// ��������� �������� �����.
		int year; unsigned int month, day;	// ��������� ����.

		_data.getTime(hour, minute, sec, sc);
		_data.getTimeZoneOffset(zone_hour, zone_minute);
		_data.getDate(year, month, day);

		_eis_time_data.set_hour_min_sec(hour + zone_hour, minute + zone_minute, sec);

		_eis_date_data.set_year_month_day(year, month, day);

		_eis_datetime_data.set_hour_min_sec(hour + zone_hour, minute + zone_minute, sec);
		_eis_datetime_data.set_year_month_day(year, month, day);
	}
	
	oracle::occi::Timestamp _data;		//!< - ���������� ������.
	eis_date::datetime _eis_datetime_data;	//!< - ������ � ������� eis_date::datetime.
	eis_date::time _eis_time_data;		//!< - ������ � ������� eis_date::time.
	eis_date::date _eis_date_data;		//!< - ������ � ������� eis_date::date.
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
	BlobField(const ORACLE_FIELD &field, oracle::occi::Blob data) : Field(field), _data(data) {
		convertType();
	}
	
	//! ����������� �����������.
	BlobField(const BlobField &src) : Field(src), _data(src._data), _db_data(src._db_data) { }
	
	//! ���������� ������� DB::Field.
	const DB::Blob & asBlob() const;

	void assignValue(DB::Blob &value) const { value = asBlob(); }
	
	//! �����������.
	/*!
	  ������� ��������� ����.
	 */
	virtual	DB::Field * _clone() const { return new BlobField(*this); }

	//! ��������� ������ ��������� ���� CORBA::Any.
	/*!
	  ������������ ������ ��������� value.
	  \param value - ������ �� ����������, ������� ������������� ������������.
	 */
	void assignAny(CORBA::Any &value) const;
	
	//! ���������� ������ DB::Field::put().
	std::ostream & put(std::ostream &) const;

private:
	//! ��������� �������� ���� _data � ��������� ��� � ������ DB::Blob.
	void convertType() {
		int size = _data.length();
		oracle::occi::Stream * instream = _data.getStream(1, 0);
		char * buffer = new char[size];
		memset(buffer, NULL, size);
		instream->readBuffer(buffer, size);
		_db_data.setData(size, buffer);
		_data.closeStream (instream);
	}
	oracle::occi::Blob _data;	//!< - ������ ��������� �� ������.
	DB::Blob _db_data;		//!< - ������ ��� ������.
};

} // namespace DB_Oracle

#endif //__DB_Oracle_FieldHPP__

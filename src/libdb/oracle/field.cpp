#ifdef WIN32
 #include <Winsock2.h>
#endif

#include "occi.h"

#include <string>
#include <sstream>

#include "common/misc/base64.hpp"
#include "common/misc/NumericAnyConversions.hpp"
#include "libdb/oracle/field.hpp"

//------------------------------------------------------------------------------------------------------
// class : NumberField
//------------------------------------------------------------------------------------------------------

int DB_Oracle::NumberField::asLong() const  {
	if(isNull()) 
		throw DB::XDBError("Null field");
	else
		return (int)_data;
}

unsigned int DB_Oracle::NumberField::asULong() const  {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return (unsigned int)_data;
}

short DB_Oracle::NumberField::asShort() const  {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return (short)_data;
}

unsigned short DB_Oracle::NumberField::asUShort() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return (unsigned short)_data;
}

long long DB_Oracle::NumberField::asLongLong() const  {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return (long)_data;
}

unsigned long long DB_Oracle::NumberField::asULongLong() const  {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return (unsigned long)_data;
}

bool DB_Oracle::NumberField::asBool() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return !((int)_data == 0);
}

float DB_Oracle::NumberField::asFloat() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return (float)_data;
}

double DB_Oracle::NumberField::asDouble() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return (double)_data;
}

const Numeric & DB_Oracle::NumberField::asNumeric() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return _numeric_data;
}

void DB_Oracle::NumberField::assignAny(CORBA::Any &value) const {
	if(isNull())
		value.iluDeleteContent();
	else
		value <<= (long double)_data;
}

std::ostream & DB_Oracle::NumberField::put(std::ostream & os) const {
	if(isNull()) 
		os << "NULL";
	else
		os << (long double)_data;
	return os;
}

void DB_Oracle::NumberField::setData(const oracle::occi::Number &data) {
	isNull(false);
	_data = data;
	_numeric_data = Numeric(_prec - _scale, _scale, asDouble());
}


//------------------------------------------------------------------------------------------------------
// class : StringField
//------------------------------------------------------------------------------------------------------

const std::string & DB_Oracle::StringField::asString() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else 
		return _data;
}

char DB_Oracle::StringField::asChar() const {
	if(isNull() || _data.size() < 1)
		throw DB::XDBError("Null field");
	else
		return (char)_data[0];
}

void DB_Oracle::StringField::assignAny(CORBA::Any &value) const {
	if(isNull())
		value.iluDeleteContent();
	else
		value <<= _data.c_str();
}

std::ostream & DB_Oracle::StringField::put(std::ostream & os) const {
	if(isNull()) 
		os << "NULL";
	else
		os << _data;
	return os;
}



//------------------------------------------------------------------------------------------------------
// class : TimestampField
//------------------------------------------------------------------------------------------------------


const eis_date::datetime & DB_Oracle::TimestampField::asDateTime() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return _datetime;
}

const eis_date::time & DB_Oracle::TimestampField::asTime() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return _time;
}

const eis_date::date & DB_Oracle::TimestampField::asDate() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return _date;
}

void DB_Oracle::TimestampField::assignAny(CORBA::Any &value) const {
	if(isNull())
		value.iluDeleteContent();
	else
		value <<= (CommonData::DateTime)_datetime;
}

std::ostream & DB_Oracle::TimestampField::put(std::ostream & os) const {
	if(isNull()) 
		os << "NULL";
	else
		os << _datetime.format(eis_date::SQL, false);
	return os;
}

void DB_Oracle::TimestampField::setData(const oracle::occi::Timestamp &data) {
	isNull(false);
	unsigned int hour, minute, sec, sc;	// параметры времени.
	int zone_hour, zone_minute;		// параметры часового пояса.
	int year; unsigned int month, day;	// параметры даты.

	data.getTime(hour, minute, sec, sc);
	data.getTimeZoneOffset(zone_hour, zone_minute);
	data.getDate(year, month, day);

	_time.set_hour_min_sec(hour + zone_hour, minute + zone_minute, sec);
	_date.set_year_month_day(year, month, day);
	_datetime.set_hour_min_sec(hour + zone_hour, minute + zone_minute, sec);
	_datetime.set_year_month_day(year, month, day);
}



//------------------------------------------------------------------------------------------------------
// class : BlobField
//------------------------------------------------------------------------------------------------------

const DB::Blob & DB_Oracle::BlobField::asBlob() const {
	if(isNull())
		throw DB::XDBError("Null field");
	else
		return _data;
}

void DB_Oracle::BlobField::assignAny(CORBA::Any &value) const {
	if(isNull())
        	value.iluDeleteContent();
	else {
		ilu_Pickle *k = value.iluPickleValue();
		if (k->pi_len) 
			ilu_free(k->pi_bytes);
		k->pi_len = _data.length;
		k->pi_bytes = (ilu_bytes)ilu_malloc(k->pi_len);
		memcpy(k->pi_bytes, _data.data, k->pi_len);
		value.iluGetFromPickle();	
	}
}

std::ostream & DB_Oracle::BlobField::put(std::ostream & os) const {
	if(isNull()) 
		os << "NULL";
	else
		os << _data;
	return os;
}

void DB_Oracle::BlobField::setData(const oracle::occi::Blob &data) {
	isNull(false);
	oracle::occi::Blob blob = data;
	int size = blob.length();
	oracle::occi::Stream * instream = blob.getStream(1, 0);
	char * buffer = new char[size];
	memset(buffer, NULL, size);
	instream->readBuffer(buffer, size);
	try {
		base64::Decoder dec((const base64::byte *)buffer, size);
		_data.setData(dec.getlength(), (const char *)dec.getbuffer());
	} catch (const base64::XDecodeFail &x) {
		_data.setData(0, NULL);
	}
	blob.closeStream(instream);
	delete[] buffer;
}

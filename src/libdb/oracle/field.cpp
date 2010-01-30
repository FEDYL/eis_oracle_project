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
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (int)_data;
}

unsigned int DB_Oracle::NumberField::asULong() const  {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (unsigned int)_data;
}

short DB_Oracle::NumberField::asShort() const  {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (short)_data;
}

unsigned short DB_Oracle::NumberField::asUShort() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (unsigned short)_data;
}

long long DB_Oracle::NumberField::asLongLong() const  {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (long)_data;
}

unsigned long long DB_Oracle::NumberField::asULongLong() const  {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (unsigned long)_data;
}

bool DB_Oracle::NumberField::asBool() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return !((int)_data == 0);
}

float DB_Oracle::NumberField::asFloat() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (float)_data;
}

double DB_Oracle::NumberField::asDouble() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return (double)_data;
}

const Numeric & DB_Oracle::NumberField::asNumeric() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return Numeric(_prec, _scale, (double)_data);
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



//------------------------------------------------------------------------------------------------------
// class : StringField
//------------------------------------------------------------------------------------------------------

const std::string & DB_Oracle::StringField::asString() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return _data;
}

char DB_Oracle::StringField::asChar() const {
	if(isNull() || _data.size() < 1) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
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
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return _eis_datetime_data;
}

const eis_date::time & DB_Oracle::TimestampField::asTime() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return _eis_time_data;
}

const eis_date::date & DB_Oracle::TimestampField::asDate() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return _eis_date_data;
}

void DB_Oracle::TimestampField::assignAny(CORBA::Any &value) const {
	if(isNull())
		value.iluDeleteContent();
	else
		value <<= (CommonData::DateTime)_eis_datetime_data;
}

std::ostream & DB_Oracle::TimestampField::put(std::ostream & os) const {
	if(isNull()) 
		os << "NULL";
	else
		os << _eis_datetime_data.format(eis_date::SQL, false);
	return os;
}



//------------------------------------------------------------------------------------------------------
// class : BlobField
//------------------------------------------------------------------------------------------------------

const DB::Blob & DB_Oracle::BlobField::asBlob() const {
	if(isNull()) {
		DB::XDBError err;
		err << "Attempt to use a NULL value, by getting data of field \"" << table() << '.' << name() << "\".";
		throw err;
	} else
		return _db_data;
}

void DB_Oracle::BlobField::assignAny(CORBA::Any &value) const {
	if(isNull())
        	value.iluDeleteContent();
	else {
		ilu_Pickle *k = value.iluPickleValue();
		if (k->pi_len) 
			ilu_free(k->pi_bytes);
		k->pi_len = _db_data.length;
		k->pi_bytes = (ilu_bytes)ilu_malloc(k->pi_len);
		memcpy(k->pi_bytes, _db_data.data, k->pi_len);
		value.iluGetFromPickle();	
	}
}

std::ostream & DB_Oracle::BlobField::put(std::ostream & os) const {
	if(isNull()) 
		os << "NULL";
	else
		os << _db_data;
	return os;
}
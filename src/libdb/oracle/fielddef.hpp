#ifndef __DB_Oracle_FieldDefHPP__
#define __DB_Oracle_FieldDefHPP__

#include <string>

#include "libdb/common/fielddef.hpp"
#include "common/misc/datetime.hpp"

namespace DB_Oracle {

typedef DB::SimpleFieldDef<int> _LongFieldDef;
//! Определение целого знакового поля.
/*!
  Реализация интерфейса DB::SimpleFieldDef для целого знакового.
 */
class LongFieldDef : public _LongFieldDef {
public:
	LongFieldDef(const std::string &name)
		: _LongFieldDef(name, "NUMBER(24)") { }

	LongFieldDef(const std::string &name, int default_value, bool null = false)
		: _LongFieldDef(name, "NUMBER(24)", default_value, null) { }
	virtual ~LongFieldDef() { }
};


typedef DB::SimpleFieldDef<unsigned int> _ULongFieldDef;
//! Определение целого беззнакового поля.
/*!
  Реализация интерфейса DB::SimpleFieldDef для целого беззнакового.
 */
class ULongFieldDef : public _ULongFieldDef {
public:
	ULongFieldDef(const std::string &name)
		: _ULongFieldDef(name, "NUMBER(24)") { }
	ULongFieldDef(const std::string &name, unsigned int default_value, bool null = false)
		: _ULongFieldDef(name, "NUMBER(24)", default_value, null) { }
	virtual ~ULongFieldDef() { }
};


typedef DB::SimpleFieldDef<short> _ShortFieldDef;
//! Определение котороткого целого знакового поля.
/*!
  Реализация интерфейса DB::SimpleFieldDef для короткого целого знакового.
 */
class ShortFieldDef : public _ShortFieldDef {
public:
	ShortFieldDef(const std::string &name)
		: _ShortFieldDef(name, "NUMBER(12)") { }
	ShortFieldDef(const std::string &name, short default_value, bool null = false)
		: _ShortFieldDef(name, "NUMBER(12)", default_value, null) { }
	virtual ~ShortFieldDef() { }
};


typedef DB::SimpleFieldDef<unsigned short> _UShortFieldDef;
//! Определение короткого целого беззнакового поля.
/*!
  Реализация интерфейса DB::SimpleFieldDef для короткого целого беззнакового.
 */
class UShortFieldDef : public _UShortFieldDef {
public:
	UShortFieldDef(const std::string &name)
		: _UShortFieldDef(name, "NUMBER(12)") { }
	UShortFieldDef(const std::string &name, unsigned short default_value, bool null = false)
		: _UShortFieldDef(name, "NUMBER(12)", default_value, null) { }
	virtual ~UShortFieldDef() { }
};


typedef DB::SimpleFieldDef<long long> _LongLongFieldDef;
//! Определение длинного знакового целого поля.
/*!
  Реализация интерфейса DB::SimpleFieldDef для длинного целого знакового.
 */
class LongLongFieldDef : public _LongLongFieldDef {
public:
	LongLongFieldDef(const std::string &name)
		: _LongLongFieldDef(name, "NUMBER(38)") { }
	LongLongFieldDef(const std::string &name, long long default_value, bool null = false)
		: _LongLongFieldDef(name, "NUMBER(38)", default_value, null) { }
	virtual ~LongLongFieldDef() { }
};


typedef DB::SimpleFieldDef<unsigned long long> _ULongLongFieldDef;
//! Определение длинного целого беззнакового поля.
/*!
  Реализация интерфейса DB::SimpleFieldDef для длинного целого беззнакового.
 */
class ULongLongFieldDef : public _ULongLongFieldDef {
public:
	ULongLongFieldDef(const std::string &name)
		: _ULongLongFieldDef(name, "NUMBER(38)") { }
	ULongLongFieldDef(const std::string &name, unsigned long long default_value, bool null = false)
		: _ULongLongFieldDef(name, "NUMBER(38)", default_value, null) { }
	virtual ~ULongLongFieldDef() { }
};


typedef DB::SimpleFieldDef<float> _FloatFieldDef;
//! Определение поля с плавающей точкой.
/*!
  Реализация интерфейса DB::SimpleFieldDef для типа с плавающей точкой.
 */
class FloatFieldDef : public _FloatFieldDef {
public:
	FloatFieldDef(const std::string &name)
		: _FloatFieldDef(name, "NUMBER(12, 5)") { }
	FloatFieldDef(const std::string &name, float default_value, bool null = false)
		: _FloatFieldDef(name, "NUMBER(12, 5)", default_value, null) { }
	virtual ~FloatFieldDef() { }
};


typedef DB::SimpleFieldDef<double> _DoubleFieldDef;
//! Определение поля с плавающей точкой, удвоенной точности.
/*!
  Реализация интерфейса DB::SimpleFieldDef для типа с плавающей точкой удвоенной точности.
 */
class DoubleFieldDef : public _DoubleFieldDef {
public:
	DoubleFieldDef(const std::string &name)
		: _DoubleFieldDef(name, "NUMBER(38, 15)") { }
	DoubleFieldDef(const std::string &name, double default_value, bool null = false)
		: _DoubleFieldDef(name, "NUMBER(38, 15)", default_value, null) { }
	virtual ~DoubleFieldDef() { }
};


typedef DB::SimpleFieldDef<char> _CharFieldDef;
//! Определение поля символьного типа (размером в 1 символ).
/*!
  Реализация интерфейса DB::SimpleFieldDef для символьного типа длиной в 1 символ.
 */
class CharFieldDef : public _CharFieldDef {
public:
	CharFieldDef(const std::string &name)
		: _CharFieldDef(name, "VARCHAR2(1)") { }
	CharFieldDef(const std::string &name, char default_value, bool null = false)
		: _CharFieldDef(name, "VARCHAR2(1)", default_value, null) { }
	virtual ~CharFieldDef() { }
};


typedef DB::SimpleFieldDef<bool> _BoolFieldDef;
//! Определение логического поля.
/*!
  Реализация интерфейса DB::SimpleFieldDef для логиского типа.
 */
class BoolFieldDef : public _BoolFieldDef {
public:
	BoolFieldDef(const std::string &name)
		: _BoolFieldDef(name, "NUMBER(1)") { }
	BoolFieldDef(const std::string &name, bool default_value, bool null = false)
		: _BoolFieldDef(name, "NUMBER(1)", default_value, null) { }
	virtual ~BoolFieldDef() { }
};


//! Определенеи строчного поля.
/*!
  Реализация интерфейса FieldDef для строчного типа.
 */
class StringFieldDef : public DB::FieldDef {
public:
	StringFieldDef(const std::string &name, unsigned int length = 255)
		: FieldDef(name), m_length(length) { }
	StringFieldDef(const std::string &name, std::string default_value, unsigned int length = 255, bool null = false);
	virtual ~StringFieldDef() { }

	//! Реализация метода FieldDef::addSelfToCreateScript().
	void addSelfToCreateScript(DB::Query & script) {
		script << m_name << " VARCHAR2(" << m_length << ')' << nullAsString() << m_default_value.asString();
	}
	
private:
	DB::DefaultValue<std::string> m_default_value;	//!< - значение по умолчанию.
	unsigned int m_length;				//!< - длина поля.
};


//! Определение BLOB-поля.
/*!
  Реализация интерфейса FieldDef для Blob-типа.
 */
class BlobFieldDef : public DB::FieldDef {
public:
	BlobFieldDef(const std::string & name, bool null = true) : FieldDef(name, null) { }
	virtual ~BlobFieldDef() { }
	
	//! Релизация метода FieldDef::addSelfToCreateScript().
	void addSelfToCreateScript(DB::Query & script) {
		script << m_name << " BLOB" << nullAsString();
	}
};


typedef DB::SimpleFieldDef<eis_date::date> _DateFieldDef;
//! Опеределение поля типа дата.
/*!
  Реализация интерфейса DB::SimpleFieldDef для типа дата.
 */
class DateFieldDef : public _DateFieldDef {
public:
	DateFieldDef(const std::string &name)
		: _DateFieldDef(name, "TIMESTAMP") { }
	DateFieldDef(const std::string &name, eis_date::date default_value, bool null = false)
		: _DateFieldDef(name, "TIMESTAMP", default_value, null) { }
	virtual ~DateFieldDef() { }

	void setDefault(const char * value) {
	        m_default_value = eis_date::date::from_format_string(value, eis_date::SQL);
	}
};


typedef DB::SimpleFieldDef<eis_date::time> _TimeFieldDef;
//! Определение поля типа время.
/*!
  Реализация интерфейса DB::SimpleFieldDef для типа время.
 */
class TimeFieldDef : public _TimeFieldDef {
public:
	TimeFieldDef(const std::string &name)
		: _TimeFieldDef(name, "TIMESTAMP") { }
	TimeFieldDef(const std::string &name, eis_date::time default_value, bool null = false)
		: _TimeFieldDef(name, "TIMESTAMP", default_value, null) { }
	virtual ~TimeFieldDef() { }

	void setDefault(const char * value) {
        	m_default_value = eis_date::time::fromString(value, false);
	}
};


typedef DB::SimpleFieldDef<eis_date::datetime> _DateTimeFieldDef;
//! Определение поля типа дата-время.
/*!
  Реализация интерфейса DB::SimpleFieldDef для типа дата-время.
 */
class DateTimeFieldDef : public _DateTimeFieldDef {
public:
	DateTimeFieldDef(const std::string &name)
		: _DateTimeFieldDef(name, "TIMESTAMP") { }
	DateTimeFieldDef(const std::string &name, eis_date::datetime default_value, bool null = false)
		: _DateTimeFieldDef(name, "TIMESTAMP", default_value, null) { }
	virtual ~DateTimeFieldDef() { }

	void setDefault(const char * value) {
        	m_default_value = eis_date::datetime::from_format_string(value, eis_date::SQL, false);
	}
};


typedef DB::SimpleFieldDef<Numeric> _NumericFieldDef;
//! Определение поля типа Numeric.
/*!
  Реализация интерфейса DB::SimpleFieldDef для типа Numeric.
 */
class NumericFieldDef : public DB::FieldDef {
public:
	NumericFieldDef(const std::string &name, unsigned int size, unsigned int prec) :
			DB::FieldDef(name), m_default_value(Numeric(size, prec)), m_size(size), m_prec(prec) {
		validate(size, prec);
	}
	NumericFieldDef(const std::string &name, Numeric default_value, unsigned int size, unsigned int prec, bool null = false) :
			DB::FieldDef(name, null), m_default_value(default_value), m_size(size), m_prec(prec) {
		validate(size, prec);
	}
	virtual ~NumericFieldDef() { }

	//! Реализация метода FieldDef::addSelfToCreateScript().
	void addSelfToCreateScript(DB::Query &script) {
		script << m_name << " NUMBER" << '(' << m_size << ',' << m_prec << ')';
	}

private:
	DB::DefaultValue<Numeric> m_default_value;	//!< - значение по умолчанию.
	unsigned int m_size;				//!< - разрядность числа.
	unsigned int m_prec;				//!< - точность (число знаков после десятичной точки).

	void validate(unsigned int size, unsigned int prec) {
		if(size > 65) {
			std::ostringstream description;
			description << "Error by creating NUMBER(" << size << ", " << prec << ") field, size must be <= 65 digits";
			throw DB::XDBError(description.str().c_str());
		}
		if(prec > 30) {
			std::ostringstream description;
			description << "Error by creating NUMBER(" << size << ", " << prec << ") field, precision must be <= 30 digits";
			throw DB::XDBError(description.str().c_str());
		}
		if(size < prec) {
			std::ostringstream description;
			description << "Error by creating NUMBER(" << size << ", " << prec << ") field, size < precision";
			throw DB::XDBError(description.str().c_str());
		}
	}
};

} // namespace DB_Oracle

#endif //__DB_Oracle_FieldDefHPP__

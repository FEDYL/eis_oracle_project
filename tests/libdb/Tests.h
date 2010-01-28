#ifndef __TestsH__
#define __TestsH__


#include "libdb/libdb.hpp"
#include "common/misc/datetime.hpp"
#include "common/misc/refcounter.hpp"

#include <iomanip>

#include "libdb/oracle/factory.hpp"
#include "libdb/oracle/fielddef.hpp"

//! Данные для подключения к бд.
#define HOST ""
#define USER "system"
#define PASS "12345"

//! Макросы вывода информации о процессе выполнения тестов.
/*! Для наглядности по большей части. */
#define PRINT_INFO_BEGIN(x) { std::cout << std::setfill('.') << std::setiosflags(std::ios::left) << std::setw(40) << x; }
#define PRINT_INFO_END      { std::cout << "yes" << std::endl; }
#define PRINT_INFO(x)       { std::cout << x << std::endl; }

void testDataBase(bool isDropOldDB = false);
void testForeignKey(bool isDropOldDB = false);
void testOCCI(bool isDropOldDB = false);

#endif //__TestsH__

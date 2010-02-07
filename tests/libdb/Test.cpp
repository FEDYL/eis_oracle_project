#include "Tests.h"

//! Параметры вызова тестов.
/*! 
  \param -b - тест базы данных (создание, редактирование, выборка, удаление бд);
  \param -f - тест работы внешних ключей (поведение при удалении и модернизации строки,
              на которую ссылаются другие);
  \param -o - тест работы OCCI.
  \param -i - тест работы итератора для DB::ResultSet.
 */

int main(int argc, char * argv[]) {
	if (argc > 1) 
		for(int i = 1; i < argc; ++i) {
			if(argv[i][0] == '-' && argv[i][1] == 'b') {
				testDataBase();
				return 0;
			}
			else if(argv[i][0] == '-' && argv[i][1] == 'f') {
				testForeignKey();
				return 0;
			}
			else if(argv[i][0] == '-' && argv[i][1] == 'o') {
				testOCCI();
				return 0;
			}
			else if(argv[i][0] == '-' && argv[i][1] == 'i') {
				testIterator();
				return 0;
			}
			else
				std::cout << "Unknown argument: " << argv[i] << std::endl;
		}
	testDataBase();
	return 0;
}

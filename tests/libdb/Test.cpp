#include "Tests.h"

//! Параметры вызова тестов.
/*! 
  \param -b - позволяет прогнать только тест базы данных (создание, редактирование, выборка, удаление бд);
  \param -f - позволяет прогнать только тест работы внешних ключей (поведение при удалении и модернизации строки,
              на которую ссылаются другие);
  \param -o - позволяет прогнать только тест работы OCCI.
 */

int main(int argc, char * argv[]) {
	if (argc > 1) 
		for(int i = 0; i < argc; ++i) {
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
		}
	testDataBase();
	return 0;
}

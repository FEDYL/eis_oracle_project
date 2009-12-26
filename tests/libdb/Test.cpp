#include "Tests.h"

//! Параметры вызова тестов.
/*! 
  \param -d - позволяет удалить уже существующую базу данных, если она не была удалена предыдущим прогоном тестов. 
              Если этот параметр есть, он должен быть первым;
  \param -b - позволяет прогнать только тест базы данных (создание, редактирование, выборка, удаление бд);
  \param -f - позволяет прогнать только тест работы внешних ключей (поведение при удалении и модернизации строки,
              на которую ссылаются другие);
  \param -o - позволяет прогнать только тест работы OCCI.
  При отсутствии параметров -b, -o и -f выполняется только основной тест базы данных (-d учитывается).
 */

int main(int argc, char * argv[]) {
	bool isDropOldDB = false;
	if (argc > 1) 
		for(int i = 1; i < argc; ++i) {
			if (argv[i][0] == '-' && argv[i][1] == 'd')
				isDropOldDB = true;
			else if(argv[i][0] == '-' && argv[i][1] == 'b') {
				testDataBase(isDropOldDB);
				return 0;
			}
			else if(argv[i][0] == '-' && argv[i][1] == 'f') {
				testForeignKey(isDropOldDB);
				return 0;
			}
			else if(argv[i][0] == '-' && argv[i][1] == 'o') {
				testOCCI();
				return 0;
			}
		}
	testDataBase(isDropOldDB);
	return 0;
}

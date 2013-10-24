/*
 *  CUnit �e�X�g���j�b�g
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Console.h"

int main(int argc, char* argv[])
{
  void unit_heap_AddTests();
  
	setvbuf(stdout, NULL, _IONBF, 0);
	if (initialize_registry()) {
		printf("\nInitialize of test Registry failed.");
    return 1;
	}

  // �e�X�g���j�b�g�̓o�^
  unit_heap_AddTests();

  // �e�X�g�̎��s
	console_run_tests();
	cleanup_registry();

	return 0;
}

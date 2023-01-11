#include <stdio.h>
#include <crtdbg.h>

#include "Test.h"
#include "UI.h"

// Program entry point
int main()
{
	// Run the tests
	runAllTests();

	// Init program
	ProductRepo* repo = createRepo();
	Service* serv = createService(repo, 1);

	UI* ui = createUI(serv);

	startUI(ui);
	destroyUI(ui);

	// Check for leaks
	if (_CrtDumpMemoryLeaks() == 1)
		printf("WARNING: Memory leaks were detected. See the Output tab for more information.\n");
	else
		printf("INFO: No memory leaks were detected. The program executed correctly.\n");

	return 0;
}

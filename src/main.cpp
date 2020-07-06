#include <logerr>

#include <mainWindow.h>

MAIN

	MainWindow w;
CrashAndBurn();
	RUN_ONCE_STARTED
	(
		w.initialize();
		w.show();
	);

END_MAIN
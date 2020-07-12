#include <logerr>

#include <mainWindow.h>

MAIN

	MainWindow w;

	RUN_ONCE_STARTED(w.initialize(););
	RUN_ONCE_STARTED(w.show(););
	RUN_ONCE_STARTED(LOGINFO << "Line 1" << std::endl;);
	RUN_ONCE_STARTED(LOGINFO << "Line 2" << std::endl;);

END_MAIN
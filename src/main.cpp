#include <logerr.h>
#include <mainWindow.h>
#include <QTimer>

void fail()
{

}

MAIN

	MainWindow w;

	QTimer::singleShot(0, [&] 
	{ 
		w.initialize(); 
		w.show(); 
	});


END_MAIN
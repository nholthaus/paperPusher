#include <logerr>
#include <logerrGuiApplication.h>
#include <mainWindow.h>

int main(int argc, char** argv)
{
	LOGERR_GUI_APP_BEGIN

	MainWindow w;

	RUN_ONCE_STARTED(w.initialize(););
	RUN_ONCE_STARTED(w.show(););
	RUN_ONCE_STARTED(LOGINFO << "Line 1" << std::endl;);
	RUN_ONCE_STARTED(LOGINFO << "Line 2" << std::endl;);
	RUN_ONCE_STARTED(LOGWARNING << "OMG Shits bad" << std::endl;);
	RUN_ONCE_STARTED(LOGDEBUG << "Look no more bugs" << std::endl;);

	LOGERR_GUI_APP_END
}
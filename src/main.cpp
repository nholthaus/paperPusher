#include <logerr.h>
#include <QDebug>

void generateTrace()
{
	StackTrace trace(7);
	QString details = QString("Crashed!") + trace;
	LOGINFO << details.toStdString() << std::endl;
}

MAIN
{
	LOGINFO << APPINFO::buildHostname().toStdString() << std::endl;
	CrashAndBurn();
}
END_MAIN
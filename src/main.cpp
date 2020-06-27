#include <logerr.h>
#include <QDebug>

MAIN
{
	QString trace = StackTrace();
LOGINFO("Hello, World!");
	qDebug() << trace;
}
END_MAIN
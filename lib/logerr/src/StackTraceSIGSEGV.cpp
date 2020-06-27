
#include <StackTrace.h>
#include <appinfo.h>
#include <QApplication>
#include <QDir>
#include <logerrMacros.h>

//--------------------------------------------------------------------------------------------------
//	stackTraceSigSev (public ) [static ]
//--------------------------------------------------------------------------------------------------
void stackTraceSIGSEGV(int sig)
{
	// Gather the details
#ifdef _MSC_VER
	StackTrace trace(7); // determined empirically
#else
	StackTrace trace(2); // determined empirically
#endif

	QString time = "\n\nTIME:\n\n";
	time += "    Start Time   : " + APPINFO::applicationStartTime().toString() + "\n";
	time += "    Crash Time   : " + QDateTime::currentDateTime().toString() + "\n";
	time += "\n";

	QString sDetails = QString("%1 Crashed! :'(").arg(APPINFO::name()) + time + APPINFO::systemDetails() + QString("STACK TRACE:\n\n") + trace;
	LOGERR << sDetails.toLocal8Bit().constData() << std::endl;

	// make sure the directory exists
	QDir dir;
	dir.mkdir(APPINFO::crashDumpDir());

	QString crashdumpFileName = QString("crashdump-") + QDateTime::currentDateTime().toString(Qt::ISODate).remove(':') + ".txt";
	if (!qApp->applicationName().isEmpty())
		crashdumpFileName.prepend(qApp->applicationName() + '-');

	// write a dedicated crash dump file too for good measure
	QFile crashDumpFile(APPINFO::crashDumpDir() + '/' + crashdumpFileName);
	crashDumpFile.open(QIODevice::WriteOnly);
	crashDumpFile.write(sDetails.toLocal8Bit());
	crashDumpFile.close();

	std::exit(1);
}

//--------------------------------------------------------------------------------------------------
//	CrashAndBurn (public ) []
//--------------------------------------------------------------------------------------------------
void CrashAndBurn()
{
	int* crash = nullptr;
	*crash = 0xDEAD;
}


#include <Application.h>
#include <logerr.h>
#include <ExceptionDialog.h>
#include <QDebug>

//--------------------------------------------------------------------------------------------------
//	Application (public ) []
//--------------------------------------------------------------------------------------------------
Application::Application(int& argc, char* argv[])
	: QApplication(argc, argv)
{

}

//--------------------------------------------------------------------------------------------------
//	~Application (public ) []
//--------------------------------------------------------------------------------------------------
Application::~Application()
{

}

//--------------------------------------------------------------------------------------------------
//	notify () []
//--------------------------------------------------------------------------------------------------
bool Application::notify(QObject* object, QEvent* event)
{
	bool retVal = false;

	try
	{
		retVal = QApplication::notify(object, event);

		// rethrow exceptions from threads 
		std::exception_ptr exceptionPtr = g_exceptionPtr;
		g_exceptionPtr = nullptr;

		if (exceptionPtr)
			std::rethrow_exception(exceptionPtr);
	}
	catch (const StackTraceException& e)
	{
		qDebug() << e.what();
		ExceptionDialog dialog(e);
		dialog.exec();

		if (e.fatal())
			throw e;
	}
	catch (const std::exception& e)
	{
	}
	catch (...)
	{
	}

	return retVal;
}


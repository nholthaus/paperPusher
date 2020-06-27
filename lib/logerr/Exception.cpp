
#include "Exception.h"
#include <QByteArray>
#include <Ver.h>
#include "Logging.h"
#include <QMetaType>
#include <Qt>
#include <QDateTime>
#include <QSysInfo>
#include <QHostInfo>

DECLARE_LOG_SRC("Exception", LOGCAT_Borg)




Exception::Exception(unsigned int stackIgnore)
	: m_msg(QString())
	, m_lineNumber(0)
	, m_timestamp(QDateTime::currentDateTime().toString(Qt::ISODate))
	, m_trace(StackTrace(stackIgnore + 1)) // ignore this classes creation in the stack trace
{
}

Exception::Exception(QString msg, unsigned int stackIgnore) : Exception(stackIgnore + 1)
{
	m_msg = msg;
}

Exception::Exception(QString msg, QString sSource, int iLineNum, unsigned int stackIgnore) : Exception(msg, stackIgnore + 1)
{
	m_moduleName = sSource;
	m_lineNumber = iLineNum;
}

Exception::Exception(const char* msg, unsigned int stackIgnore /*= 1*/) : Exception(stackIgnore + 1)
{
	m_msg = msg;
}

Exception::Exception(const char** msg, unsigned int stackIgnore /*= 1*/) : Exception(stackIgnore + 1)
{
	m_msg = *msg;
}



void Exception::setInfo(QString origin, QString file, uint lineNumber, QString moduleName)
{
	m_origin = origin;
	m_file = file;
	m_lineNumber = lineNumber;
	m_moduleName = moduleName;
}

QString Exception::Msg() const
{
	return m_msg;
}


QString Exception::Origin() const
{
	return m_origin;
}

uint Exception::LineNumber() const
{
	return m_lineNumber;
}

QString Exception::ModuleName() const
{
	return m_moduleName;
}

QString Exception::ToString() const
{
	return QString("Exception: %1 | From: %2 (line: %3)").arg(
					m_msg,
					m_origin,
					QString::number(m_lineNumber));
}

//--------------------------------------------------------------------------------------------------
//	ToDetailedString ( public )
//--------------------------------------------------------------------------------------------------
QString Exception::ToDetailedString() const
{
	QString details;
	details.append("EXCEPTION DETAILS:\n");
	details.append(QString("    Timetamp     : %1\n").arg(Timestamp()));
	details.append(QString("    Msg          : %1\n").arg(Msg()));
	details.append(QString("    Origin       : %1\n").arg(Origin()));
	details.append(QString("    File         : %1\n").arg(File()));
	details.append(QString("    Line         : %1\n").arg(QString::number(LineNumber())));
	details.append(QString("    Error Code   : %1\n").arg(m_sErrorCode));
	details.append("\n");
	details.append(m_sysDetails);
	details.append(QString("STACK TRACE:\n%1\n").arg(Trace()));

	return details;
}


QString Exception::ErrorCode() const
{
	return m_sErrorCode;
}


void Exception::SetMsg(const QString& sMsg, const QString& sErrCode)
{
	m_msg = sMsg;
	m_sErrorCode = sErrCode;
}

void Exception::SetMsg(const QString& sMsg)
{
	m_msg = sMsg;
}

QString Exception::Trace() const
{
	return m_trace;
}

QString Exception::File() const
{
	return m_file;
}

QString Exception::Timestamp() const
{
	return m_timestamp;
}

const char* Exception::what() const throw()
{
	((Exception *)this)->m_AsciiMsg = QString("%1\n On: %2:%3").
		arg(m_msg, m_origin, QString::number(m_lineNumber)).toLatin1();

	return m_AsciiMsg.constData();
}


/*******************************************************************************/

ExceptionContainer::ExceptionContainer(std::exception_ptr ex)
{
	m_Exception = ex;

	try
	{
		Rethrow();
	}
	catch(Exception& ex) // This can't be a const ref because class name isn't a const reference
	{
		LOGINFO("Creating ExceptionContainer for %s", qPrintable(ex.GetType()));
	}
	catch(...)
	{
		// This exception is not a type ExceptionContainer supports
		Q_ASSERT(false);
	}
}

QString ExceptionContainer::Msg() const
{
	try
	{
		Rethrow();
	}
	catch(const Exception& ex)
	{
		return ex.Msg();
	}
	catch(...)
	{
		Q_ASSERT(0);
		return QString();
	}

	return QString();
}


void ExceptionContainer::Rethrow() const
{
	std::rethrow_exception(m_Exception);
}

ExceptionContainer ExceptionContainer::CurrentException()
{
	std::exception_ptr ptr = std::current_exception();
	return ExceptionContainer(ptr);
}

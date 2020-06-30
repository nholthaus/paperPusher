#include <StackTraceException.h>
#include <appinfo.h>
#include <appinfo.h>

//--------------------------------------------------------------------------------------------------
//	StackTraceException (public ) []
//--------------------------------------------------------------------------------------------------
StackTraceException::StackTraceException(QString errorMessage, QString filename, size_t line, bool fatal)
	: m_errorMessage(errorMessage)
	, m_fileName(filename)
	, m_line(line)
	, m_trace(StackTrace(1))
	, m_fatal(fatal)
{
	m_what = QString("ERROR: ")
		.append(m_errorMessage)
		.append(" at `")
		.append(m_fileName)
		.append(':')
		.append(QString::number(line))
		.append("`\n\n")
		.append(APPINFO::systemDetails())
		.append("STACK TRACE:\n\n")
		.append(m_trace);

	if (fatal)
		m_what.prepend("FATAL ");
}

//--------------------------------------------------------------------------------------------------
//	what (public ) []
//--------------------------------------------------------------------------------------------------
char const* StackTraceException::what() const
{
	return m_what.toLocal8Bit().constData();
}

//--------------------------------------------------------------------------------------------------
//	filename (public ) []
//--------------------------------------------------------------------------------------------------
QString StackTraceException::filename() const
{
	return m_fileName;
}

//--------------------------------------------------------------------------------------------------
//	errorMessage (public ) []
//--------------------------------------------------------------------------------------------------
QString StackTraceException::errorMessage() const
{
	return m_errorMessage;
}

//--------------------------------------------------------------------------------------------------
//	line (public ) []
//--------------------------------------------------------------------------------------------------
size_t StackTraceException::line() const
{
	return m_line;
}

//--------------------------------------------------------------------------------------------------
//	trace (public ) []
//--------------------------------------------------------------------------------------------------
QString StackTraceException::trace() const
{
	return m_trace;
}

//--------------------------------------------------------------------------------------------------
//	fatal (public ) []
//--------------------------------------------------------------------------------------------------
bool StackTraceException::fatal() const
{
	return m_fatal;
}


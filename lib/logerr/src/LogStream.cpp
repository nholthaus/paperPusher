//------------------------------
//	INCLDUES
//------------------------------
#include <LogStream.h>
#include <ostream>

Q_DECLARE_METATYPE(std::string)

//--------------------------------------------------------------------------------------------------
//	LogStream () []
//--------------------------------------------------------------------------------------------------
LogStream::LogStream(std::ostream& stream) 
	: m_stream(stream)
	, m_old_buf(stream.rdbuf())
{
	qRegisterMetaType<std::string>("std::string");
	stream.rdbuf(this);
}

//--------------------------------------------------------------------------------------------------
//	~LogStream () []
//--------------------------------------------------------------------------------------------------
LogStream::~LogStream()
{
	// output anything that is left
	if (!m_string.empty())
		emit logEntryReady(m_string);

	m_stream.rdbuf(m_old_buf);
}

//--------------------------------------------------------------------------------------------------
//	overflow () []
//--------------------------------------------------------------------------------------------------
std::basic_streambuf<char>::int_type LogStream::overflow(int_type v)
{
	m_string += v;

	if (v == '\n')
	{
		emit logEntryReady(m_string);
		m_string.clear();
	}

	return v;
}

//--------------------------------------------------------------------------------------------------
//	xsputn (protected ) [virtual ]
//--------------------------------------------------------------------------------------------------
std::streamsize LogStream::xsputn(const char* p, std::streamsize n)
{
	m_string.append(p, p + n);

	size_t pos = 0;
	while (pos != std::string::npos)
	{
		pos = m_string.find('\n');
		if (pos != std::string::npos)
		{
			std::string tmp(m_string.begin(), m_string.begin() + pos + 1);
			emit logEntryReady(tmp);
			m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
		}
	}

	return n;
}


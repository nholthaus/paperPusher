#pragma once
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QDataStream>
#include <qstringlist.h>
#include <exception>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include "StackTrace.h"
#include "SystemDetails.h"

// Hacky work around template / className removed DynObj fun
#define DECLARE_EXCEPTION_NAME(exname) \
	static QString Type() { return QStringLiteral("ex" #exname); } \
	virtual QString GetType() const { return QStringLiteral("ex" #exname); }



/**
@brief The base class of all exceptions.

This class is thrown by reference everywere. You would not likely need
to use this class directly. Instead, use the Logging.h EXERR/EXWRN/EXINFO/EXERR_T
macros to throw an exception and log the message.

Every exception has text. Not all of this text would make sense to a customer/user.
But if an unexpected program condition did cause an exception, the user may see it.
For this reason, there is an additional UserMsg() method.  Only exceptions thrown
with the bUserMsg flag set true will return the actual text from UserMsg(). Otherwise,
it will return something like 'Internal Error'. It would be great to figure
out how to auto-generate a log message ID so that an internal error shown to
the user is easy to remotely correlate to a log message.
*/

class Exception : public std::exception
{
public:
	Exception(unsigned int stackIgnore = 1);
	Exception(const char* msg, unsigned int stackIgnore = 1);
	Exception(const char** msg, unsigned int stackIgnore = 1);
	Exception(QString msg, unsigned int stackIgnore = 1);
	Exception(QString msg, QString sSource, int iLineNum, unsigned int stackIgnore = 1); //< Need to use this constructor to get correct error codes.

	enum ErrorCodesResult{
		ERROR_CODES_SUCCESS,
		ERROR_CODES_INVALID_SIZE,
		ERROR_CODES_CHECKSUM_FAIL,
	};

	QString Msg() const;		///< The message of the exception, filled in at throw-time
	QString ErrorCode() const;  ///< Error code of the exception.
	QString Origin() const;
	uint LineNumber() const;
	QString ModuleName() const;
	QString File() const;
	QString Trace() const;		///< Stack trace that generated the exception
	QString Timestamp() const;
	QString ToString() const;
	QString ToDetailedString() const;

	void SetMsg(const QString& sMsg);
	void SetMsg(const QString& sMsg, const QString& sErrCode);

	/// Setup the extra info to the exception for various reasons
	void setInfo(QString origin, QString file, uint lineNumber, QString moduleName);

	const char* what() const throw() override;


	DECLARE_EXCEPTION_NAME(Exception)

	operator QString() const { return GetType(); }

	friend QDataStream& operator<< (QDataStream& s, const Exception& other);
	friend QDataStream& operator>> (QDataStream& s, Exception& other);

protected:
	QString m_msg;
	QString m_origin;
	QString m_moduleName;
	uint m_lineNumber;
	QByteArray m_AsciiMsg;
	QString m_sErrorCode;
	QString m_file;
	QString m_timestamp;
	SystemDetails m_sysDetails;
	StackTrace m_trace;
};

// (Why? Because we are using it with QtRPC2)
// these don't need to be serializable, so we just have straight up declare
// the << and >> operators for datastreams
// Hendrick: I thought of making this object a public SerMig but there is an
//           issue with the order of includes due to ArchiveException et al
//           so there would be a circular reference that I don't think it's
//           worth dealing with today
inline QDataStream& operator<< (QDataStream& s, const Exception& other)
{
	s << other.m_msg;
	s << other.m_origin;
	s << other.m_moduleName;
	s << other.m_lineNumber;
	s << other.m_AsciiMsg;
	return s;
}
inline QDataStream& operator>> (QDataStream& s, Exception& other)
{
	s >> other.m_msg;
	s >> other.m_origin;
	s >> other.m_moduleName;
	s >> other.m_lineNumber;
	s >> other.m_AsciiMsg;
	return s;
}


/**
@brief Container class for passing Exceptions cross threads
*/
class ExceptionContainer
{
public:
	ExceptionContainer() = default;

	static ExceptionContainer CurrentException();

	QString Msg() const;
	void Rethrow() const;

	template <typename T>
	static ExceptionContainer CreateExceptionContainer(const T& ex)
	{
		static_assert(std::is_base_of<Exception, T>::value, "CreateException needs to be used with a type derived from Exception");
		auto ep = std::make_exception_ptr<T>(ex);
		return ExceptionContainer(ep);
	}

	template <typename T>
	static ExceptionContainer CreateExceptionContainer(const QString & msg)
	{
		static_assert(std::is_base_of<Exception, T>::value, "CreateException needs to be used with a type derived from Exception");
		auto ep = std::make_exception_ptr<T>(msg);
		return ExceptionContainer(ep);
	}

private:
	ExceptionContainer(std::exception_ptr ex);
	std::exception_ptr m_Exception;
};
Q_DECLARE_METATYPE(ExceptionContainer)

//------------------------------
//	MACROS
//------------------------------

#define DECLARE_EXCEPTION_TYPE(type) \
class type : public Exception \
{ \
public: \
	type() {} \
	type(QString msg) : Exception(msg) {} \
	DECLARE_EXCEPTION_NAME(type) \
};

#endif // EXCEPTION_H

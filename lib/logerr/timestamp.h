//--------------------------------------------------------------------------------------------------
// 
//	Timestamp: unit-based timestamping library
//
//--------------------------------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files (the "Software"), to deal in the Software without 
// restriction, including without limitation the rights to use, copy, modify, merge, publish, 
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or 
// substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//--------------------------------------------------------------------------------------------------
// 
// Copyright (c) 2016 Nic Holthaus
// 
//--------------------------------------------------------------------------------------------------
//
// ATTRIBUTION:
// Parts of this work have been adapted from: 
//
//--------------------------------------------------------------------------------------------------
//
/// @file	timestamp.h
/// @brief	returns a timestamp string
//
//--------------------------------------------------------------------------------------------------

#ifndef timestamp_h__
#define timestamp_h__

//------------------------
//	INCLUDES
//------------------------

#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdint>
#include <string>
#include <iostream>

#include <QString>
#include <QDateTime>

#undef pascal
#include <units.h>

//	----------------------------------------------------------------------------
//	CLASS		Timestamp
//  ----------------------------------------------------------------------------
///	@brief		Functor class that stores a timestamp.
///	@details	By design, the Timestamp is implicitly convertible to a bunch
///				of different c, c++, and string formats. You can use the timestamp
///				like a function by calling Timestamp(), or you can create a timestamp
///				instance, which allows you to review the timestamp value at a later
///				point in the code.\n\n
//  ---------------------------------------------------------------------------- 
class Timestamp
{
public:
	
	Timestamp() : m_now(std::chrono::system_clock::now())
	{

	}
	
	/// @returns difference in milliseconds
	inline units::time::millisecond_t operator-(Timestamp t)
	{
		return units::time::nanosecond_t((double)std::chrono::duration_cast<std::chrono::nanoseconds>(this->m_now - t.m_now).count());
	}

	/// @returns difference in milliseconds
	inline units::time::millisecond_t operator-(std::chrono::system_clock::time_point t)
	{
		return units::time::nanosecond_t((double)std::chrono::duration_cast<std::chrono::nanoseconds>(this->m_now - t).count());
	}

	/// @returns difference in milliseconds
	inline units::time::millisecond_t operator-(uint64_t t)
	{
		return units::time::nanosecond_t((double)std::chrono::duration_cast<std::chrono::nanoseconds>(this->m_now.time_since_epoch()).count() - t);
	}

	/// Updates the timestamp to the current time
	inline Timestamp& update()
	{
		m_now = std::chrono::system_clock::now();
		return *this;
	}

	/// Updates the timestamp, returning the elapsed time since the creation/update of the timestamp
	inline units::time::millisecond_t elapsed()
	{
		auto before = *this;
		return update() - before;
	}

	template<typename T>
	inline T to() const
	{
		return static_cast<T>(*this);
	}

	/// @returns an integer representing the millisecond part of the timestamp
	inline std::chrono::milliseconds fractionalPart() const
	{
		return std::chrono::milliseconds(this->to<uint64_t>() % 1000);
	}

	inline std::chrono::seconds seconds_since_epoch() const
	{
		return std::chrono::duration_cast<std::chrono::seconds>(m_now.time_since_epoch());
	}

	inline operator std::time_t() const
	{
		return std::chrono::system_clock::to_time_t(m_now);
	}
	
	inline operator std::chrono::system_clock::time_point() const
	{
		return m_now;
	}
	
	operator std::string() const
	{

	#pragma warning(push)
	#pragma warning(disable: 4996)	// disable: warning C4996: 'localtime': This function or variable may be unsafe. Consider using localtime_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	
		char buffer[128];
		std::time_t now_c = *this;
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
		
		std::string seconds(buffer);
		std::string nanoseconds = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(m_now.time_since_epoch()).count() % 1000000000);
		
		// have to add any leading zeros back in
		nanoseconds = std::string(9 - nanoseconds.length(), '0') + nanoseconds;
		
		return seconds + '.' + nanoseconds;
	#pragma warning(pop)
	}
	
	/**
	 * @returns		time since the linux epoch in milliseconds
	 */
	inline operator std::chrono::milliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_now.time_since_epoch());
	}

	inline operator units::time::millisecond_t() const
	{
		return units::time::millisecond_t((double)std::chrono::duration_cast<std::chrono::milliseconds>(m_now.time_since_epoch()).count());
	}

	// Returns the time as a QDateTime
	inline operator QDateTime() const 
	{
		return QDateTime::fromMSecsSinceEpoch((std::chrono::duration_cast<std::chrono::milliseconds>(m_now.time_since_epoch()).count()));
	}

	// returns the time as a QString
	inline operator QString() const
	{
		QString nanoseconds = QString("%1").arg(std::chrono::duration_cast<std::chrono::nanoseconds>(m_now.time_since_epoch()).count() % 1000000000, 9, 10, QChar('0'));
		return  static_cast<QDateTime>(*this).toString("yyyy-MM-dd hh:mm:ss.") + nanoseconds;
	}

	/**
	 * @returns		time since the linux epoch in milliseconds
	 */
	inline operator std::uint64_t() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_now.time_since_epoch()).count();
	}
	
	inline friend std::ostream& operator<<(std::ostream& os, const Timestamp& t)
	{
		os << static_cast<std::string>(t);
		return os;
	}

private:
	
	std::chrono::system_clock::time_point m_now;
};

inline QDebug operator<<(QDebug d, units::time::millisecond_t t)
{
	d << QString::fromStdString(units::time::to_string(t));
	return d;
}

inline QDebug operator<<(QDebug d, Timestamp t)
{
	d << static_cast<QString>(t);
	return d;
}
#endif // timestamp_h__

// For Emacs
// Local Variables:
// Mode: C++
// c-basic-offset: 2
// fill-column: 116
// tab-width: 4
// End:
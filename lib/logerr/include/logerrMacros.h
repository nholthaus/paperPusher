//--------------------------------------------------------------------------------------------------
// 
//	LOGERR
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
// Copyright (c) 2020 Nic Holthaus
// 
//--------------------------------------------------------------------------------------------------
//
// ATTRIBUTION:
//
//
//--------------------------------------------------------------------------------------------------
//
/// @file	logerrMacros.h
/// @brief	Macro Definitions for the logerr library
//
//--------------------------------------------------------------------------------------------------

#pragma once
#ifndef logerrMacros_h__
#define logerrMacros_h__

//-------------------------
//	INCLUDES
//-------------------------

#include <iostream>
#include <appinfo.h>
#include <StackTraceSIGSEGV.h>

//-------------------------
//	HELPER FUNCTIONS
//-------------------------

namespace logerr
{
	template<class T>
	constexpr std::string printable(const T& value)
	{
		if constexpr (std::is_same_v<QString, std::remove_cv_t<T>>) 
			return value.toStdString();
		else if constexpr (std::is_same_v<std::string, std::remove_cv_t<T>>) return value;
//		else if constexpr (std::is_same_v<char*, std::remove_cv_t<T>>) return value;
//		else if constexpr (std::is_convertible_v<const char*, std::remove_cv_t<T>>) return static_cast<const char*>(value);
//		else if constexpr (std::is_convertible_v<std::string, std::remove_cv_t<T>>) return std::string(value).data();
	}
}

//-------------------------
//	MACROS
//-------------------------

// LOG FUNCTIONS
#ifndef LOGERR
#define LOGERR		std::cout << "[ERROR]    "
#endif
#ifndef LOGWARNING
#define LOGWARNING	std::cout << "[WARNING]  "
#endif
#ifndef LOGDEBUG
#define LOGDEBUG	std::cout << "[DEBUG]    "
#endif
#ifndef LOGINFO
#define LOGINFO		std::cout << "[INFO]     "
#endif

// MAIN
#ifndef MAIN
#define MAIN \
int main(int argc, char* argv[]) \
{\
	LOGINFO << logerr::printable(APPINFO::name()) << ' ' << logerr::printable(APPINFO::version()) << " Started" << std::endl;\
	std::signal(SIGSEGV, stackTraceSIGSEGV);\
	\
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);\
	\
	QApplication app(argc,argv);\
	app.setOrganizationName(APPINFO::organization());\
	app.setOrganizationDomain(APPINFO::organizationDomain());\
	app.setApplicationName(APPINFO::name());\
	app.setApplicationVersion(APPINFO::version());
#endif

// END_MAIN
#ifndef END_MAIN
#define END_MAIN \
	LOGINFO << logerr::printable(APPINFO::name()) << " Exited Successfully" << std::endl;\
}
#endif

#endif // logerrMacros_h__

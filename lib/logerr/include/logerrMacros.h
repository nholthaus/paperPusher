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
#include <thread>
#include <exception>
#include <appinfo.h>
#include <Application.h>
#include <StackTraceException.h>
#include <StackTraceSIGSEGV.h>

//------------------------------
//	GLOBALS
//------------------------------

static std::exception_ptr g_exceptionPtr = nullptr;
static std::thread::id g_mainThreadID;

//-------------------------
//	HELPER FUNCTIONS
//-------------------------

namespace logerr
{
	template<class T>
	constexpr std::string printable(const T& value)
	{
		if constexpr (std::is_same_v<QString, std::remove_cv_t<T>>) return value.toStdString();
		else if constexpr (std::is_same_v<std::string, std::remove_cv_t<T>>) return value;
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

// filename
#ifndef  __FILENAME__
#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#endif 

// errors
#ifndef ERR
#define ERR(msg) std::this_thread::get_id() == g_mainThreadID ? throw StackTraceException(msg, __FILENAME__, __LINE__) : g_exceptionPtr = std::make_exception_ptr(StackTraceException(msg, __FILENAME__, __LINE__))
#endif

#ifndef FATAL_ERR
#define FATAL_ERR(msg) std::this_thread::get_id() == g_mainThreadID ? throw StackTraceException(msg, __FILENAME__, __LINE__, true) : g_exceptionPtr = std::make_exception_ptr(StackTraceException(msg, __FILENAME__, __LINE__, true))
#endif

// verify
#ifndef VERIFY
#define VERIFY(condition) ((!(condition)) ? qt_assert(#condition, __FILENAME__, __LINE__) : qt_noop())
#endif

// TODO
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "(" __STR1__(__LINE__) "): TODO - "

#ifdef Q_OS_WIN
#define TODO(x) __pragma(message(__LOC__ x))
#else
#define TODO(x)
#endif

// MAIN
#ifndef MAIN
#define MAIN \
int main(int argc, char* argv[]) \
{ \
	try \
	{ \
		LOGINFO << logerr::printable(APPINFO::name()) << ' ' << logerr::printable(APPINFO::version()) << " Started." << std::endl; \
		std::signal(SIGSEGV, stackTraceSIGSEGV); \
		\
		g_mainThreadID = std::this_thread::get_id(); \
		\
		QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); \
		\
		Application app(argc,argv); \
		app.setOrganizationName(APPINFO::organization()); \
		app.setOrganizationDomain(APPINFO::organizationDomain()); \
 		app.setApplicationName(APPINFO::name()); \
 		app.setApplicationVersion(APPINFO::version());
#endif

// END_MAIN
#ifndef END_MAIN
#define END_MAIN \
	} \
	catch(StackTraceException& e) \
	{ \
		LOGERR << e.what() << std::endl; \
		LOGINFO << logerr::printable(APPINFO::name()) << " exiting due to fatal error..." << std::endl;  \
		std::exit(2); \
	} \
	catch(std::exception& e) \
	{ \
		LOGERR << "ERROR: " << e.what() << std::endl; \
		LOGINFO << logerr::printable(APPINFO::name()) << " exiting due to fatal error..." << std::endl;  \
		std::exit(2); \
	} \
	catch(...) \
	{ \
		LOGERR << "ERROR: An unknown fatal error occured. " << std::endl; \
		LOGINFO << logerr::printable(APPINFO::name()) << " exiting due to fatal error..." << std::endl;  \
		std::exit(2); \
	} \
	\
	LOGINFO << logerr::printable(APPINFO::name()) << " Exited Successfully" << std::endl;\
	return 0; \
}
#endif

#endif // logerrMacros_h__

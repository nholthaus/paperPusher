//--------------------------------------------------------------------------------------------------
// 
//	
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
/// @file	StackTraceSigSev.h
/// @brief	
//
//--------------------------------------------------------------------------------------------------

#pragma once
#ifndef StackTraceSigSev_h__
#define StackTraceSigSev_h__

//-------------------------
//	INCLUDES
//-------------------------

#include <csignal>

//-------------------------
//	FORWARD DECLARATIONS
//-------------------------

class QString;

//--------------------------------------------------------------------------------------------------
//	FUNCTIONS
//--------------------------------------------------------------------------------------------------

// Provides a c++ signal handler that will generate a stack trace upon crashing
void stackTraceSIGSEGV(int sig);

// This function intentionally crashes the program, 
// for test purposes.
void CrashAndBurn();

#endif // StackTraceSigSev_h__

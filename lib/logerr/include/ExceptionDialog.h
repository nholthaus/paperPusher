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
/// @file	ExceptionDialog.h
/// @brief	
//
//--------------------------------------------------------------------------------------------------

#pragma once
#ifndef ExceptionDialog_h__
#define ExceptionDialog_h__

//-------------------------
//	INCLUDES
//-------------------------

#include <QDialog>
#include <QTextBrowser>

//-------------------------
//	FORWARD DECLARATIONS
//-------------------------

class StackTraceException;
class QPixmap;
class QLabel;
class QGroupBox;
class QHBoxLayout;
class QVBoxLayout;

//--------------------------------------------------------------------------------------------------
//	CorrectlySizedTextBrowser
//--------------------------------------------------------------------------------------------------
class CorrectlySizedTextBrowser : public QTextBrowser
{
public:
	CorrectlySizedTextBrowser(QWidget* parent = nullptr);;
	virtual QSize sizeHint() const override;
	virtual QSize minimumSizeHint() const override;
};

//--------------------------------------------------------------------------------------------------
//	ExceptionDialog
//--------------------------------------------------------------------------------------------------
class ExceptionDialog : public QDialog
{
	Q_OBJECT

public:

	ExceptionDialog(const StackTraceException& msg, QWidget* parent = nullptr);
	virtual ~ExceptionDialog();

private slots:

	void on_pbCopy_clicked();
	void on_pbExit_clicked();
	void on_pbDetails_clicked();
	void on_pbReportBug_clicked();

private:

	QString						m_errorMessage;
	QString						m_errorDetails;
	QString						m_filename;
	QString						m_line;

	QPixmap*					m_errorPixmap;
	QLabel*						m_errorMessageLabel;
	QLabel*						m_errorLocationLabel;
	QGroupBox*					m_detailsGroupBox;
	CorrectlySizedTextBrowser*	m_detailsTextBrowser;

	QPushButton*				m_showDetailsButton;
	QPushButton*				m_copyButton;
	QPushButton*				m_okButton;

	QVBoxLayout*				m_topLayout;
	QHBoxLayout*				m_errorLayout;
	QHBoxLayout*				m_buttonLayout;
};


#endif // ExceptionDialog_h__







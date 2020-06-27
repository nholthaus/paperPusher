#ifndef EXCEPTIONDIALOG_H
#define EXCEPTIONDIALOG_H

#include <QDialog>
#include "Exception.h"

namespace Ui 
{
	class ExceptionDialog;
	class CorrectlySizedTextBrowser;
};

class CorrectlySizedTextBrowser : public QTextBrowser
{
public:
	CorrectlySizedTextBrowser(QWidget* parent = nullptr) : QTextBrowser(parent) 
	{
		VERIFY(connect(this, &CorrectlySizedTextBrowser::textChanged, this, &CorrectlySizedTextBrowser::updateGeometry));
	};

	virtual QSize sizeHint() const override
	{
		auto font = this->currentFont();
		auto textSize = QFontMetrics(font).size(0, this->document()->toPlainText());
		int newWidth = textSize.width() + 30;
		return QSize(newWidth, QTextBrowser::sizeHint().height());
	}

	virtual QSize minimumSizeHint() const override
	{
		return sizeHint();
	}

};

class ExceptionDialog : public QDialog
{
	Q_OBJECT

public:
	ExceptionDialog(QWidget *parent = 0, const Exception& msg = Exception());
	~ExceptionDialog();

	static void ShowModal(const ExceptionContainer & ex, QWidget *parent = 0);

	Ui::ExceptionDialog *ui;

private slots:
	void on_pbCopy_clicked();
	void on_pbExit_clicked();
	void on_pbDetails_clicked();
	void on_pbReportBug_clicked();

private:
	Exception m_ex;
	static ExceptionDialog* m_pModalDialog;
};

#endif // EXCEPTIONDIALOG_H

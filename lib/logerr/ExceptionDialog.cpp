#include "stdafx.h"
#include "ExceptionDialog.h"
#include "ui_ExceptionDialog.h"
#include "FileBug.h"

ExceptionDialog* ExceptionDialog::m_pModalDialog = nullptr;

ExceptionDialog::ExceptionDialog(QWidget *parent, const Exception& ex)
	: QDialog(parent)
{
	// Save it for later
	m_ex = ex;

	ui = new Ui::ExceptionDialog();
	ui->setupUi(this);

	ui->labelMessage->setText(ex.Msg());
	ui->labelLocationInfo->setText(QString("at: %1 (%2:%3)").arg(ex.Origin()).arg(ex.File()).arg(ex.LineNumber()));
	ui->labelErrorCode->setText(QString("Error Code: %1").arg(ex.ErrorCode()));
	ui->labelMessage->setTextInteractionFlags(Qt::TextSelectableByMouse);
	ui->tbStackTrace->setText(ex.ToDetailedString());
	ui->tbStackTrace->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	ui->tbStackTrace->verticalScrollBar()->setSingleStep(QFontMetrics(ui->tbStackTrace->font()).height()/2);
	ui->gbDetails->setVisible(false);

	auto cur = ui->tbStackTrace->textCursor();
	cur.setPosition(ex.ToDetailedString().indexOf("STACK TRACE:"));
	ui->tbStackTrace->setTextCursor(cur);

	QApplication::clipboard()->setText(m_ex.ToDetailedString());
}

ExceptionDialog::~ExceptionDialog()
{
	delete ui;
}


void ExceptionDialog::ShowModal(const ExceptionContainer& ex, QWidget *parent)
{
	try
	{
		ex.Rethrow();
	}
	catch(const Exception& ex)
	{
        m_pModalDialog = new ExceptionDialog(parent, ex);
		m_pModalDialog->adjustSize();
		m_pModalDialog->exec();
		delete m_pModalDialog;
		m_pModalDialog = nullptr;
	}
}

void ExceptionDialog::on_pbExit_clicked()
{
	this->close();
}

void ExceptionDialog::on_pbDetails_clicked()
{
	if (ui->pbDetails->isChecked())
	{
		ui->pbDetails->setText("Hide Details");
		ui->gbDetails->setVisible(true);		
	}
	else
	{
		ui->pbDetails->setText("Show Details");
		ui->gbDetails->setVisible(false);
	}

	if (m_pModalDialog)
		m_pModalDialog->adjustSize();

	// scroll to the stack trace
	//ui->tbStackTrace->ensureCursorVisible();
}

//--------------------------------------------------------------------------------------------------
//	on_pbReportBug_clicked ( private )
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbReportBug_clicked()
{
	QScopedPointer<FileBug> bugDialog(new FileBug);
	bugDialog->SetSystemDetails(m_ex.ToDetailedString());
	bugDialog->setModal(true);
	bugDialog->exec();
	ui->pbExit->click();
}

void ExceptionDialog::on_pbCopy_clicked()
{
	QApplication::clipboard()->setText(m_ex.ToDetailedString());
}


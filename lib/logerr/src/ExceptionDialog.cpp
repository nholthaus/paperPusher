 #include <ExceptionDialog.h>
#include <logerr.h>

#include <QApplication>
#include <QFontDatabase>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

ExceptionDialog::ExceptionDialog(const StackTraceException& exception, QWidget* parent /*= nullptr*/)
	: QDialog(parent)
	, m_errorMessage(exception.errorMessage())
	, m_errorDetails(exception.what())
	, m_filename(exception.filename())
	, m_line(QString::number(exception.line()))
	, m_errorMessageLabel(new QLabel(m_errorMessage, this))
	, m_errorLocationLabel(new QLabel(QString("%1:%2").arg(m_filename).arg(m_line), this))
	, m_detailsGroupBox(new QGroupBox("Details"))
	, m_detailsTextBrowser(new CorrectlySizedTextBrowser(this))
	, m_showDetailsButton(new QPushButton("Show Details", this))
	, m_copyButton(new QPushButton("Copy Error", this))
	, m_okButton(new QPushButton("OK", this))
	, m_topLayout(new QVBoxLayout)
{
	this->setWindowTitle("ERROR");
	this->setLayout(m_topLayout);
	m_topLayout
// 	ui->labelMessage->setText(ex.Msg());
// 	ui->labelLocationInfo->setText(QString("at: %1 (%2:%3)").arg(ex.Origin()).arg(ex.File()).arg(ex.LineNumber()));
// 	ui->labelErrorCode->setText(QString("Error Code: %1").arg(ex.ErrorCode()));
// 	ui->labelMessage->setTextInteractionFlags(Qt::TextSelectableByMouse);
// 	ui->tbStackTrace->setText(ex.ToDetailedString());
// 	ui->tbStackTrace->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
// 	ui->tbStackTrace->verticalScrollBar()->setSingleStep(QFontMetrics(ui->tbStackTrace->font()).height()/2);
// 	ui->gbDetails->setVisible(false);
// 
// 	auto cur = ui->tbStackTrace->textCursor();
// 	cur.setPosition(ex.ToDetailedString().indexOf("STACK TRACE:"));
// 	ui->tbStackTrace->setTextCursor(cur);
// 
// 	QApplication::clipboard()->setText(m_ex.ToDetailedString());
}

ExceptionDialog::~ExceptionDialog()
{

}


//--------------------------------------------------------------------------------------------------
//	on_pbCopy_clicked (public ) []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbCopy_clicked()
{
	QApplication::clipboard()->setText(m_exception);
}

//--------------------------------------------------------------------------------------------------
//	on_pbExit_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbExit_clicked()
{

}

//--------------------------------------------------------------------------------------------------
//	on_pbDetails_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbDetails_clicked()
{

}

//--------------------------------------------------------------------------------------------------
//	on_pbReportBug_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbReportBug_clicked()
{

}

// void ExceptionDialog::ShowModal(const ExceptionContainer& ex, QWidget *parent)
// {
// 	try
// 	{
// 		ex.Rethrow();
// 	}
// 	catch(const Exception& ex)
// 	{
//         m_pModalDialog = new ExceptionDialog(parent, ex);
// 		m_pModalDialog->adjustSize();
// 		m_pModalDialog->exec();
// 		delete m_pModalDialog;
// 		m_pModalDialog = nullptr;
// 	}
// }
// 
// void ExceptionDialog::on_pbExit_clicked()
// {
// 	this->close();
// }
// 
// void ExceptionDialog::on_pbDetails_clicked()
// {
// 	if (ui->pbDetails->isChecked())
// 	{
// 		ui->pbDetails->setText("Hide Details");
// 		ui->gbDetails->setVisible(true);		
// 	}
// 	else
// 	{
// 		ui->pbDetails->setText("Show Details");
// 		ui->gbDetails->setVisible(false);
// 	}
// 
// 	if (m_pModalDialog)
// 		m_pModalDialog->adjustSize();
// 
// 	// scroll to the stack trace
// 	//ui->tbStackTrace->ensureCursorVisible();
// }


// void ExceptionDialog::on_pbCopy_clicked()
// {
// 	QApplication::clipboard()->setText(m_ex.ToDetailedString());
// }

//--------------------------------------------------------------------------------------------------
//	CorrectlySizedTextBrowser (public ) []
//--------------------------------------------------------------------------------------------------
CorrectlySizedTextBrowser::CorrectlySizedTextBrowser(QWidget* parent /*= nullptr*/) 
	: QTextBrowser(parent)
{
	VERIFY(connect(this, &CorrectlySizedTextBrowser::textChanged, this, &CorrectlySizedTextBrowser::updateGeometry));
}

//--------------------------------------------------------------------------------------------------
//	sizeHint (public ) []
//--------------------------------------------------------------------------------------------------
QSize CorrectlySizedTextBrowser::sizeHint() const
{
	auto font = this->currentFont();
	auto textSize = QFontMetrics(font).size(0, this->document()->toPlainText());
	int newWidth = textSize.width() + 30;
	return QSize(newWidth, QTextBrowser::sizeHint().height());
}

//--------------------------------------------------------------------------------------------------
//	minimumSizeHint (public ) []
//--------------------------------------------------------------------------------------------------
QSize CorrectlySizedTextBrowser::minimumSizeHint() const
{
	return sizeHint();
}

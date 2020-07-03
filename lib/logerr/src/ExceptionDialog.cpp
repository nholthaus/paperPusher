 #include <ExceptionDialog.h>
#include <logerr.h>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QScrollBar>
#include <QStyle>
#include <QHBoxLayout>
#include <QVBoxLayout>

ExceptionDialog::ExceptionDialog(const StackTraceException& exception, QWidget* parent /*= nullptr*/)
	: QDialog(parent)
	, m_errorMessage(exception.errorMessage())
	, m_errorDetails(exception.errorDetails())
	, m_filename(exception.filename())
	, m_line(QString::number(exception.line()))
	, m_errorIcon(new QLabel(this))
	, m_errorMessageLabel(new QLabel(m_errorMessage.prepend("ERROR: "), this))
	, m_errorLocationLabel(new QLabel(QString("at: %1:%2").arg(m_filename).arg(m_line), this))
	, m_detailsGroupBox(new QGroupBox())
	, m_detailsGroupBoxLayout(new QVBoxLayout)
	, m_detailsTextBrowser(new CorrectlySizedTextBrowser(this))
	, m_detailsButtonLayout(new QHBoxLayout)
	, m_applicationInfoButton(new QPushButton("App Info"))
	, m_versionInfoButton(new QPushButton("Version Info"))
	, m_buildInfoButton(new QPushButton("Build Info"))
	, m_hostInfoButton(new QPushButton("Host Info"))
	, m_StackTraceButton(new QPushButton("Stack Trace"))
	, m_showDetailsButton(new QPushButton("Show Details", this))
	, m_copyButton(new QPushButton("Copy Error", this))
	, m_okButton(new QPushButton("OK", this))
	, m_topLayout(new QVBoxLayout)
	, m_errorLayout(new QHBoxLayout)
	, m_buttonLayout(new QHBoxLayout)
{
	qDebug() << m_errorDetails;
	this->setWindowTitle("ERROR");
	this->setLayout(m_topLayout);

	m_topLayout->addLayout(m_errorLayout);
	m_topLayout->addWidget(m_detailsGroupBox);
	m_topLayout->addLayout(m_buttonLayout);

	m_errorLayout->addWidget(m_errorIcon);
	m_errorIcon->setPixmap(style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(QSize(50, 50)));

	m_errorLayout->addWidget(m_errorMessageLabel);
	m_errorMessageLabel->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	m_errorLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Maximum));

	m_detailsGroupBox->setLayout(m_detailsGroupBoxLayout);
	m_detailsGroupBoxLayout->addWidget(m_errorLocationLabel);
	m_detailsGroupBoxLayout->addWidget(m_detailsTextBrowser);
	m_detailsGroupBoxLayout->addLayout(m_detailsButtonLayout);
	m_detailsGroupBox->setHidden(true);

	m_detailsButtonLayout->addWidget(m_applicationInfoButton);
	m_detailsButtonLayout->addWidget(m_versionInfoButton);
	m_detailsButtonLayout->addWidget(m_buildInfoButton);
	m_detailsButtonLayout->addWidget(m_hostInfoButton);
	m_detailsButtonLayout->addWidget(m_StackTraceButton);

	m_buttonLayout->addWidget(m_showDetailsButton);
	m_buttonLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding, QSizePolicy::Maximum));
	m_buttonLayout->addWidget(m_copyButton);
	m_buttonLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Maximum));
	m_buttonLayout->addWidget(m_okButton);

	m_errorMessageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
 	m_detailsTextBrowser->setText(m_errorDetails);
	m_detailsTextBrowser->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	m_detailsTextBrowser->verticalScrollBar()->setSingleStep(QFontMetrics(m_detailsTextBrowser->font()).height()/2);

	VERIFY(connect(m_showDetailsButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbDetails_clicked));
	VERIFY(connect(m_copyButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbCopy_clicked));
	VERIFY(connect(m_okButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbOK_clicked));

	VERIFY(connect(m_applicationInfoButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbApplicationInfoButton_clicked));
	VERIFY(connect(m_versionInfoButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbVersionInfoButton_clicked));
	VERIFY(connect(m_buildInfoButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbBuildInfoButton_clicked));
	VERIFY(connect(m_hostInfoButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbHostInfoButton_clicked));
	VERIFY(connect(m_StackTraceButton, &QPushButton::clicked, this, &ExceptionDialog::on_pbStackTraceButton_clicked));
}

ExceptionDialog::~ExceptionDialog()
{

}

//--------------------------------------------------------------------------------------------------
//	on_pbCopy_clicked (public ) []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbCopy_clicked()
{
	QApplication::clipboard()->setText(m_errorDetails);
}

//--------------------------------------------------------------------------------------------------
//	on_pbExit_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbOK_clicked()
{
	this->close();
}

//--------------------------------------------------------------------------------------------------
//	on_pbDetails_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbDetails_clicked()
{
	m_detailsGroupBox->setHidden(!m_detailsGroupBox->isHidden());

	if (!m_detailsGroupBox->isHidden())
		m_StackTraceButton->click();

	this->adjustSize();
}

//--------------------------------------------------------------------------------------------------
//	on_pbApplicationInfoButton_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbApplicationInfoButton_clicked()
{
	auto cur = m_detailsTextBrowser->document()->find("APPLICATION INFO:");
	m_detailsTextBrowser->moveCursor(QTextCursor::End);
	m_detailsTextBrowser->setTextCursor(cur);
}

//--------------------------------------------------------------------------------------------------
//	on_pbVersionInfoButton_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbVersionInfoButton_clicked()
{
	auto cur = m_detailsTextBrowser->document()->find("VERSION INFO:");
	m_detailsTextBrowser->moveCursor(QTextCursor::End);
	m_detailsTextBrowser->setTextCursor(cur);
}

//--------------------------------------------------------------------------------------------------
//	on_pbBuildInfoButton_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbBuildInfoButton_clicked()
{
	auto cur = m_detailsTextBrowser->document()->find("BUILD INFO:");
	m_detailsTextBrowser->moveCursor(QTextCursor::End);
	m_detailsTextBrowser->setTextCursor(cur);
}

//--------------------------------------------------------------------------------------------------
//	on_pbHostInfoButton_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbHostInfoButton_clicked()
{
	auto cur = m_detailsTextBrowser->document()->find("HOST INFO:");
	m_detailsTextBrowser->moveCursor(QTextCursor::End);
	m_detailsTextBrowser->setTextCursor(cur);
}

//--------------------------------------------------------------------------------------------------
//	on_pbStackTraceButton_clicked () []
//--------------------------------------------------------------------------------------------------
void ExceptionDialog::on_pbStackTraceButton_clicked()
{
	auto cur = m_detailsTextBrowser->document()->find("STACK TRACE:");
	m_detailsTextBrowser->moveCursor(QTextCursor::End);
	m_detailsTextBrowser->setTextCursor(cur);
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

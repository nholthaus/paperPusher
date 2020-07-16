#include <logDock.h>
#include <LogModel.h>
#include <LogProxyModel.h>
#include <logerr>

#include <QCheckBox>
#include <QFontDatabase>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLayout>
#include <QStyleFactory>
#include <QTreeView>
#include <QVBoxLayout>

//--------------------------------------------------------------------------------------------------
//	LogDock (public ) []
//--------------------------------------------------------------------------------------------------
LogDock::LogDock()
	: QDockWidget("Log Window")
	, m_logModel(new LogModel(this))
	, m_logProxyModel(new LogProxyModel(this))
	, m_logView(new QTreeView(this))
	, m_topLevelWidget(new QFrame(this))
	, m_topLevelLayout(new QVBoxLayout)
	, m_settingsLayout(new QHBoxLayout)
	, m_typesGroupbox(new QGroupBox("Show"))
	, m_settingsGroupBox(new QGroupBox("Settings"))
	, m_errorCheckBox(new QCheckBox("Errors"))
	, m_warningCheckBox(new QCheckBox("Warnings"))
	, m_infoCheckBox(new QCheckBox("Info"))
	, m_debugCheckBox(new QCheckBox("Debug"))
	, m_showTimestampsCheckBox(new QCheckBox("Timestamps"))
	, m_autoscrollCheckBox(new QCheckBox("Autoscroll"))
{
	QFont monospaceFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

	this->setWidget(m_topLevelWidget);

	m_topLevelWidget->setLayout(m_topLevelLayout);
	m_topLevelLayout->addLayout(m_settingsLayout);
	m_topLevelLayout->addWidget(m_logView);

	m_settingsLayout->addWidget(m_typesGroupbox);
	m_settingsLayout->addWidget(m_settingsGroupBox);
	m_settingsLayout->setContentsMargins(0,0,0,0);

	m_typesGroupbox->setLayout(new QHBoxLayout);
	m_typesGroupbox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_typesGroupbox->layout()->addWidget(m_errorCheckBox);
	m_typesGroupbox->layout()->addWidget(m_warningCheckBox);
	m_typesGroupbox->layout()->addWidget(m_infoCheckBox);
	m_typesGroupbox->layout()->addWidget(m_debugCheckBox);
	m_typesGroupbox->layout()->addWidget(m_showTimestampsCheckBox);

	m_errorCheckBox->setChecked(true);
	m_warningCheckBox->setChecked(true);
	m_infoCheckBox->setChecked(true);
	m_debugCheckBox->setChecked(true);
	m_showTimestampsCheckBox->setChecked(true);
	m_autoscrollCheckBox->setChecked(true);

	m_settingsGroupBox->setLayout(new QHBoxLayout);
	m_settingsGroupBox->layout()->addWidget(m_autoscrollCheckBox);

	m_logProxyModel->setSourceModel(m_logModel);

	m_logView->setModel(m_logProxyModel);
	m_logView->setStyle(QStyleFactory::create("fusion"));
	m_logView->setFont(monospaceFont);
	m_logView->setHeaderHidden(false);
	m_logView->setAllColumnsShowFocus(true);
	m_logView->setUniformRowHeights(true);
	m_logView->setAlternatingRowColors(true);
	m_logView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

	VERIFY(connect(m_logView->model(), &QAbstractListModel::rowsInserted, this, &LogDock::autoscroll));

	VERIFY(connect(m_showTimestampsCheckBox, &QCheckBox::toggled, this, &LogDock::on_showTimestampsCheckBox_toggled));

	VERIFY(connect(m_errorCheckBox, &QCheckBox::toggled, [this] { m_logProxyModel->setAcceptsErrors(m_errorCheckBox->isChecked()); }));
	VERIFY(connect(m_warningCheckBox, &QCheckBox::toggled, [this] { m_logProxyModel->setAcceptsWarnings(m_warningCheckBox->isChecked()); }));
	VERIFY(connect(m_infoCheckBox, &QCheckBox::toggled, [this] { m_logProxyModel->setAcceptsInfo(m_infoCheckBox->isChecked()); }));
	VERIFY(connect(m_debugCheckBox, &QCheckBox::toggled, [this] { m_logProxyModel->setAcceptsDebug(m_debugCheckBox->isChecked()); }));
}

//--------------------------------------------------------------------------------------------------
//	~LogDock (public ) []
//--------------------------------------------------------------------------------------------------
LogDock::~LogDock()
{

}

//--------------------------------------------------------------------------------------------------
//	queueLogEntry (public ) []
//--------------------------------------------------------------------------------------------------
void LogDock::queueLogEntry(std::string str)
{
	m_logModel->queueLogEntry(str);

}

//--------------------------------------------------------------------------------------------------
//	on_showTimestampsCheckBox_toggled (private ) []
//--------------------------------------------------------------------------------------------------
void LogDock::on_showTimestampsCheckBox_toggled()
{
	m_logView->setColumnHidden(0, !m_showTimestampsCheckBox->isChecked());
}

//--------------------------------------------------------------------------------------------------
//	autoscroll (private ) []
//--------------------------------------------------------------------------------------------------
void LogDock::autoscroll()
{
	if (m_autoscrollCheckBox->isChecked())
	m_logView->scrollToBottom();
}


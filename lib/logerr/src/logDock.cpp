#include <logDock.h>
#include <LogModel.h>

#include <QLayout>
#include <QTreeView>
#include <QVBoxLayout>

//--------------------------------------------------------------------------------------------------
//	LogDock (public ) []
//--------------------------------------------------------------------------------------------------
LogDock::LogDock()
	: QDockWidget("Log Window")
	, m_logModel(new LogModel(this))
	, m_logView(new QTreeView(this))
{
	this->setWidget(m_logView);
	m_logView->setModel(m_logModel);
	m_logView->setHeaderHidden(false);
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
	m_logModel->appendRow(str);
}


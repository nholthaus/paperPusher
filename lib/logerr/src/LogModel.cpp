#include <LogModel.h>
#include <timestampLite.h>

#include <QRegularExpression>

//------------------------------
//	CONSTANTS
//------------------------------

constexpr quintptr LOG_ENTRY		= -1;
constexpr const char* regex			= R"(\s*?\[(.*?)\]\s*?\[(.*?)\]\s*?(.*?)\n(.*))";

//--------------------------------------------------------------------------------------------------
//	LogModel (public ) []
//--------------------------------------------------------------------------------------------------
LogModel::LogModel(QObject* parent)
	: QAbstractItemModel(parent)
	, m_regex(regex)
{
	m_regex.setPatternOptions(QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
}

//--------------------------------------------------------------------------------------------------
//	~LogModel () []
//--------------------------------------------------------------------------------------------------
LogModel::~LogModel()
{

}

//--------------------------------------------------------------------------------------------------
//	index () []
//--------------------------------------------------------------------------------------------------
QModelIndex LogModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!this->hasIndex(row, column, parent))
		return QModelIndex();

	// handle top-level model entries - the most common case
	if (parent == QModelIndex())
	{
		if (row < m_logData.size() && column < columnCount())
			return createIndex(row, column, LOG_ENTRY);
	}
	else if (parent.isValid())
	{
		const QStringList& logEntry = m_logData[parent.row()];
		if (logEntry.size() >= columnCount(parent))
			return createIndex(row, column, parent.row());
	}

	return QModelIndex();
}

//--------------------------------------------------------------------------------------------------
//	parent () []
//--------------------------------------------------------------------------------------------------
QModelIndex LogModel::parent(const QModelIndex& child) const
{
	if (child.internalId() == LOG_ENTRY)
		return QModelIndex();
	else
		return index(child.internalId(), 0);
}

//--------------------------------------------------------------------------------------------------
//	rowCount () []
//--------------------------------------------------------------------------------------------------
int LogModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	int count = 0;

	if (parent.isValid())
		hasChildren(parent) ? count = m_logData[parent.row()].size() - columnCount() : count = 0;
	else
		count = m_logData.size();

	return count;
}

//--------------------------------------------------------------------------------------------------
//	columnCount () []
//--------------------------------------------------------------------------------------------------
int LogModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 3;
}

//--------------------------------------------------------------------------------------------------
//	hasChildren () []
//--------------------------------------------------------------------------------------------------
bool LogModel::hasChildren(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!parent.isValid())
		return !m_logData.empty();
	else if (parent.internalId() == LOG_ENTRY)
		return m_logData[parent.row()].size() > columnCount();
	else 
		return false;
}

//--------------------------------------------------------------------------------------------------
//	data () []
//--------------------------------------------------------------------------------------------------
QVariant LogModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		if (!index.parent().isValid())
			return m_logData[index.row()][index.column()];
		else if (index.column() < 2)
			return "";
		else
			return m_logData[index.parent().row()].last();
	default:
		return QVariant();
	}

}

//--------------------------------------------------------------------------------------------------
//	headerData () []
//--------------------------------------------------------------------------------------------------
QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (section)
		{
		case 0:
			return "Timestamp";
		case 1:
			return "Type";
		case 2:
			return "Details";
		default:
			return "SOMETHING WRONG";
		}
	default:
		return QVariant();
	}
}

//--------------------------------------------------------------------------------------------------
//	insertRows () []
//--------------------------------------------------------------------------------------------------
bool LogModel::insertRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
{
	assert(row >= 0);
	assert(row <= rowCount());

	this->beginInsertRows(parent, row, row + count);

	auto& insertionItr = m_logData.begin();
	std::advance(insertionItr, row);

	for (int i = 0; i < count; ++i)
		m_logData.insert(insertionItr++, QStringList());

	this->endInsertRows();

	return true;
}

//--------------------------------------------------------------------------------------------------
//	setData (public ) []
//--------------------------------------------------------------------------------------------------
bool LogModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	assert(value.type() == QVariant::String);

	if (!index.isValid())
		return false;

	m_logData[index.row()][index.column()] = value.toString();

	emit dataChanged(index, index, QVector{ role });

	return true;
}

//--------------------------------------------------------------------------------------------------
//	appendRow (public ) [virtual ]
//--------------------------------------------------------------------------------------------------
void LogModel::appendRow(const QString& value)
{
	this->beginInsertRows(QModelIndex(), rowCount(), rowCount());

	auto match = m_regex.match(value);
	
	// this can happen for raw cout writes that didn't use the macros.
	if (!match.hasMatch())
	{
		QStringList valueList = value.split('\n');
		m_logData.emplace_back(TimestampLite());
		m_logData.back().append("INFO");
		m_logData.back().append(valueList.front().trimmed());
		valueList.pop_front();
		if(!valueList.isEmpty())
			m_logData.back().append(valueList);
	}
	else
	{
		m_logData.emplace_back(match.captured(1));
		m_logData.back().append(match.captured(2));
		m_logData.back().append(match.captured(3).trimmed());
		m_logData.back().append(match.captured(4).split('\n'));
	}

	this->endInsertRows();
}

//--------------------------------------------------------------------------------------------------
//	appendRow (public ) [virtual ]
//--------------------------------------------------------------------------------------------------
void LogModel::appendRow(const std::string& value)
{
	return appendRow(QString::fromStdString(value));
}

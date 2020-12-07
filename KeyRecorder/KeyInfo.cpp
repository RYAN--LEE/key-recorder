#include "KeyInfo.h"

enum StoreKeyEnum
{
	E_KEY_ID,
	E_KEY_X,
	E_KEY_Y,
	E_KEY_INTERNAL,
	E_KEY_NEXT_ID,
	E_KEY_BREAK_ID,
	E_KEY_CONDITION,
	E_KEY_ADJUST,

	E_KEY_END,
};

KeyInfo::KeyInfo()
{
	m_adjustRect = QRect();
}
KeyInfo::KeyInfo(int id, int x, int y, int interval, int nextID, int breakID, QString& condition)
	: m_id(id)
	, m_x(x)
	, m_y(y)
	, m_adjustX(x)
	, m_adjustY(y)
	, m_interval(interval)
	, m_nextID(nextID)
	, m_breakID(breakID)
	, m_strCondition(condition)
{
	m_adjustRect = QRect();
}

KeyInfo::KeyInfo(QString& data)
{
	m_adjustRect = QRect();
	QStringList values = data.split(",");
	if (values.size() < E_KEY_END)
	{
		return;
	}

	m_id = values[E_KEY_ID].toInt();
	m_x = values[E_KEY_X].toInt();
	m_y = values[E_KEY_Y].toInt();
	m_interval = values[E_KEY_INTERNAL].toInt();
	m_nextID = values[E_KEY_NEXT_ID].toInt();
	m_breakID = values[E_KEY_BREAK_ID].toInt();
	m_strCondition = values[E_KEY_CONDITION];

	m_adjustRect = string2Rect(values[E_KEY_ADJUST]);


	m_adjustX = m_x;
	m_adjustY = m_y;
}

QString KeyInfo::string()
{
	return QString::number(m_id) + "," +
		QString::number(m_x) + "," +
		QString::number(m_y) + "," +
		QString::number(m_interval) + "," +
		QString::number(m_nextID) + "," +
		QString::number(m_breakID) + "," +
		m_strCondition + "," + 
		rect2String(m_adjustRect);
}

int KeyInfo::x()
{
	return m_x;
}

int KeyInfo::y()
{
	return m_y;
}

int KeyInfo::interval()
{
	return m_interval;
}

QString KeyInfo::condition()
{
	return m_strCondition;
}

QRect KeyInfo::getAdjustRect()
{
	return m_adjustRect;
}

void KeyInfo::setAdjustRect(QRect rect)
{
	m_adjustRect = rect;
}

QStringList KeyInfo::toStringList()
{
	QStringList stringList;
	stringList << QString().number(m_id)
		<< QString().number(m_x)
		<< QString().number(m_y)
		<< QString().number(m_interval)
		<< QString().number(m_nextID)
		<< QString().number(m_breakID)
		<< m_strCondition
		<< ""
		<< rect2String(m_adjustRect)
		;

	return stringList;
}

QRect KeyInfo::string2Rect(QString& strRect)
{
	QRect rect;
	QStringList strList = strRect.split("_");
	if (strList.size() > 0)
	{
		rect.setX(strList[0].toInt());
	}
	if (strList.size() > 1)
	{
		rect.setY(strList[1].toInt());
	}
	if (strList.size() > 2)
	{
		rect.setWidth(strList[2].toInt());
	}
	if (strList.size() > 3)
	{
		rect.setHeight(strList[3].toInt());
	}

	return rect;
}
QString KeyInfo::rect2String(QRect& rect)
{
	return QString::number(rect.x()) + "_" +
		QString::number(rect.y()) + "_" +
		QString::number(rect.width()) + "_" +
		QString::number(rect.height());
}
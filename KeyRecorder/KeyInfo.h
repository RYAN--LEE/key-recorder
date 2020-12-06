#pragma once
#include <QString>
#include <QStringList>
#include <QMetaType>
#include <QRect>

class KeyInfo
{
public:
	KeyInfo()
	{
		m_adjustRect = QRect();
	}
	KeyInfo(int id, int x, int y, int interval, int nextID, int breakID = 0, QString& condition = QString(""))
		: m_id(id)
		, m_x(x)
		, m_y(y)
		, m_interval(interval)
		, m_nextID(nextID)
		, m_breakID(breakID)
		, m_strCondition(condition)
	{
		m_adjustRect = QRect();
	}

	KeyInfo(QString& data)
	{
		m_adjustRect = QRect();
		QStringList values = data.split(",");
		if (values.size() < 7)
		{
			return;
		}

		m_id = values[0].toInt();
		m_x = values[1].toInt();
		m_y = values[2].toInt();
		m_interval = values[3].toInt();
		m_nextID = values[4].toInt();
		m_breakID = values[5].toInt();
		m_strCondition = values[6];
	}

	QString string()
	{
		return QString::number(m_id) + "," +
			QString::number(m_x) + "," +
			QString::number(m_y) + "," +
			QString::number(m_interval) + "," +
			QString::number(m_nextID) + "," +
			QString::number(m_breakID) + "," +
			m_strCondition;
	}

	int x()
	{
		return m_x;
	}

	int y()
	{
		return m_y;
	}

	int interval()
	{
		return m_interval;
	}

	QString condition()
	{
		return m_strCondition;
	}

	QStringList toStringList()
	{
		QStringList stringList;
		stringList << QString().number(m_id)
			<< QString().number(m_x) 
			<< QString().number(m_y)
			<< QString().number(m_interval)
			<< QString().number(m_nextID)
			<< QString().number(m_breakID)
			<< m_strCondition;

		return stringList;
	}

public:
	int m_id;
	int m_x;
	int m_y;
	int m_interval;
	QString m_strCondition;
	int m_nextID;
	int m_breakID;

	QRect m_adjustRect;
};

Q_DECLARE_METATYPE(KeyInfo)

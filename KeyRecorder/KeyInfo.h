#pragma once
#include <QString>
#include <QStringList>

class KeyInfo
{
public:
	KeyInfo(int x, int y, int interval, QString& condition = QString(""))
		: m_x(x)
		, m_y(y)
		, m_interval(interval)
	{
	}

	KeyInfo(QString& data)
	{
		QStringList values = data.split(",");
		if (values.size() < 3)
		{
			return;
		}

		m_x = values[0].toInt();
		m_y = values[1].toInt();
		m_interval = values[2].toInt();
	}

	QString string()
	{
		return QString::number(m_x) + "," + QString::number(m_y) + "," + QString::number(m_interval);
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

private:
	int m_x;
	int m_y;
	int m_interval;
	QString m_strCondition;
};
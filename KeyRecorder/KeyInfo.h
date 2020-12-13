#pragma once
#include <QString>
#include <QStringList>
#include <QMetaType>
#include <QRect>



class KeyInfo
{
public:
	KeyInfo();
	KeyInfo(int id, int x, int y, int interval, int nextID, 
		int breakID = 0, QString& condition = QString(""), QString& beforeCondition = QString(""));
	KeyInfo(QString& data);

	QString string();

	int x();

	int y();

	int interval();

	QString condition();
	QString beforeCondition();

	QStringList toStringList();

	QRect getAdjustRect();
	void setAdjustRect(QRect rect);

public:
	static QRect string2Rect(QString& strRect);
	static QString rect2String(QRect& rect);
public:
	int m_id;
	int m_x;
	int m_y;
	int m_interval;
	QString m_strCondition;
	int m_nextID;
	int m_breakID;
	QString m_strBeforeCondition;

	QRect m_adjustRect;
	int m_adjustX;
	int m_adjustY;
};

Q_DECLARE_METATYPE(KeyInfo)

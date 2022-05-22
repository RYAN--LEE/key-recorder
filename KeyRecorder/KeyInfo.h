#pragma once
#include <QString>
#include <QStringList>
#include <QMetaType>
#include <QRect>


class IOperate;
class KeyInfo
{
public:
	KeyInfo();
	KeyInfo(int id, int x, int y, int interval, int nextID, 
		int breakID = 0, const QString& condition = QString(""), const QString& beforeCondition = QString(""));
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

	void operate();
public:
	static QRect string2Rect(const QString& strRect);
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

	int m_needClick;
};

Q_DECLARE_METATYPE(KeyInfo)

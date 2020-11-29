#pragma once
#include "KeyInfo.h"
#include <QVector>
#include <QString>
#include <QRect>
#include <QObject>
#include <QMap>

class KeyStore : public QObject
{
public:
	KeyStore(QObject* parent);
	~KeyStore();

	//按键存储
	bool saveKeys(QVector<KeyInfo>& vecPoint);
	QVector<KeyInfo> getKeys();

	//opencv识别模板
	bool saveCVTemplate(int status, QString& strPath);

	//ocr识别区域
	bool saveRecongnizeArea(QString& type, QRect& rect);
	QRect getRecongnizeArea(QString& type);

private:
	QMap<QString, QRect> m_mapAreaRect;
};


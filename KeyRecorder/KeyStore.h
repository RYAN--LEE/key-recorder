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

	//�����洢
	bool saveKeys(QVector<KeyInfo>& vecPoint);
	QVector<KeyInfo> getKeys();

	//opencvʶ��ģ��
	bool saveCVTemplate(int status, QString& strPath);

	//ocrʶ������
	bool saveRecongnizeArea(QString& type, QRect& rect);
	QRect getRecongnizeArea(QString& type);

private:
	QMap<QString, QRect> m_mapAreaRect;
};


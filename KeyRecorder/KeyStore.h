#pragma once
#include "KeyInfo.h"
#include <QVector>
#include <QString>
#include <QRect>
#include <QObject>
#include <QMap>
#include "constant.h"

class KeyStore : public QObject
{
public:
	KeyStore(QObject* parent);
	~KeyStore();

	//�����洢
	bool saveKeys(QVector<KeyInfo>& vecPoint);
	QVector<KeyInfo> getKeys(QString path = FILE_KEY, int* errNo = nullptr);
	int loadFromFile(QString& path, QVector<KeyInfo>& retPoints, QString& errMsg);
	int copyCondition(QString& condition, QString& imgPath, QString& errMsg);
	int copyFile(QString sourceDir, QString toDir);

	//opencvʶ��ģ��
	bool saveCVTemplate(int status, QString& strPath);

	//ocrʶ������
	bool saveRecongnizeArea(QString& type, QRect& rect);
	QRect getRecongnizeArea(QString& type);

private:
	QMap<QString, QRect> m_mapAreaRect;
};


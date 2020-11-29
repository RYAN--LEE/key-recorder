#include "KeyStore.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "constant.h"

KeyStore::KeyStore(QObject* parent)
	:QObject(parent)
{

}
KeyStore::~KeyStore()
{

}

//按键存储
bool KeyStore::saveKeys(QVector<KeyInfo>& vecPoint)
{
	QFile file(FILE_KEY, this);
	if (!file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
	{
		qDebug() << "open failed";
		return false;
	}

	QTextStream out(&file);
	for (int i = 0; i < vecPoint.size(); i++)
	{
		out << vecPoint[i].string() << endl;
	}
	out.flush();

	file.close();
	return true;
}

QVector<KeyInfo> KeyStore::getKeys()
{
	QVector<KeyInfo> vecPoint;

	QFile file(FILE_KEY, this);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return vecPoint;

	QTextStream in(&file);
	QString data = in.readAll();
	file.close();

	QStringList points = data.split('\n');
	for (QList<QString>::iterator itr = points.begin(); itr != points.end(); itr++)
	{
		KeyInfo keyInfo(*itr);
		vecPoint.push_back(keyInfo);
	}

	return vecPoint;
}

//opencv识别模板
bool KeyStore::saveCVTemplate(int status, QString& strPath)
{

	return true;
}

//ocr识别区域
bool KeyStore::saveRecongnizeArea(QString& type, QRect& rect)
{
	QFile file(FILE_AREA, this);
	if (!file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
	{
		qDebug() << "open failed";
		return false;
	}

	m_mapAreaRect[type] = rect;

	QTextStream out(&file);
	for (QMap<QString, QRect>::iterator itr= m_mapAreaRect.begin(); itr!= m_mapAreaRect.end(); ++itr)
	{
		QRect rect = itr.value();
		out << itr.key() << ":" << rect.x() << "," << rect.y() << "," << rect.width() << "," << rect.height() << endl;
	}
	out.flush();

	file.close();
	return true;
}

QRect KeyStore::getRecongnizeArea(QString& type)
{
	if (!m_mapAreaRect.isEmpty())
	{
		return m_mapAreaRect[type];
	}

	QFile file(FILE_AREA, this);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return QRect();

	QTextStream in(&file);
	QString data = in.readAll();
	file.close();

	QStringList areas = data.split('\n');
	for (QList<QString>::iterator itr = areas.begin(); itr != areas.end(); itr++)
	{
		QStringList kv = itr->split(":");
		if (kv.size() != 2)
		{
			continue;
		}
		QStringList strRect = kv[1].split(",");
		if (kv.size() != 4)
		{
			continue;
		}
		QRect rect(strRect[0].toInt(), strRect[1].toInt(), strRect[2].toInt(), strRect[3].toInt());
		m_mapAreaRect[kv[0]] = rect;
	}

	return m_mapAreaRect[type];
}
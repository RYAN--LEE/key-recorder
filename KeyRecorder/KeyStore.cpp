#include "KeyStore.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

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

QVector<KeyInfo> KeyStore::getKeys(QString path, int* errNo)
{
	QVector<KeyInfo> vecPoint;

	QFile file(path, this);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		if (errNo != nullptr)
		{
			*errNo = -1;
		}
		return vecPoint;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString data = in.readLine();
		KeyInfo keyInfo(data);
		if (keyInfo.m_id == -1)
		{
			if (errNo != nullptr)
			{
				*errNo = -2;
			}
			continue;
		}
		vecPoint.push_back(keyInfo);
	}
	file.close();

	return vecPoint;
}

int KeyStore::loadFromFile(QString& path, QVector<KeyInfo>& retPoints, QString& errMsg)
{
	path.replace("\\", "/");
	if (path == FILE_KEY)
	{
		return 1;
	}
	int nRet = 0;
	QVector<KeyInfo> vecPoint = getKeys(path, &nRet);
	if (vecPoint.isEmpty())
	{
		if (nRet == -1)
		{
			errMsg = QString::fromLocal8Bit("文件打开失败");
		}
		else if (nRet == -2)
		{
			errMsg = QString::fromLocal8Bit("配置参数无效");
		}
		else
		{
			errMsg = QString::fromLocal8Bit("配置为空");
		}
		return -1;
	}

	QString imgPath = path.left(path.lastIndexOf("/")) + "/image/";
	for (int i = 0; i < vecPoint.size(); ++i)
	{
		KeyInfo key = vecPoint[i];
		nRet = copyCondition(key.beforeCondition(), imgPath, errMsg);
		if (nRet != 0)
		{
			return nRet;
		}

		nRet = copyCondition(key.condition(), imgPath, errMsg);
		if (nRet != 0)
		{
			return nRet;
		}
	}

	saveKeys(vecPoint);
	for (int i = 0; i < vecPoint.size(); ++i)
	{
		retPoints.push_back(vecPoint[i]);
	}
	return 0;
}
int KeyStore::copyCondition(QString& condition, QString& imgPath, QString& errMsg)
{
	if (condition.contains("img_"))
	{
		int nCPRet = copyFile(imgPath + condition, IMG_DIR + condition);
		if (nCPRet != 0)
		{
			if (nCPRet == -1)
			{
				errMsg = QString::fromLocal8Bit("图片文件不存在\n") + imgPath + condition;
			}
			else if (nCPRet == -2)
			{
				errMsg = QString::fromLocal8Bit("复制图片失败\n") + imgPath + condition;
			}
			return -2;
		}
	}

	return 0;
}
int KeyStore::copyFile(QString sourceDir, QString toDir)
{
	if (!QFile::exists(sourceDir)) {
		return -1;
	}
	QDir* createfile = new QDir;
	bool exist = createfile->exists(toDir);
	if (exist) {
		createfile->remove(toDir);
	}

	if (!QFile::copy(sourceDir, toDir))
	{
		return -2;

	}

	return 0;
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
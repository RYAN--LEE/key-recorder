#include "Configure.h"
#include <QFile>
#include "constant.h"
#include <QTextStream>
#include <QJsonDocument>
#include <QByteArray>
#include <QDir>


Configure::Configure()
{
	init();
	initWindowName();
}

void Configure::init()
{
	QDir* folder = new QDir;
	if (!folder->exists(HOME_DIR))
	{
		folder->mkdir(HOME_DIR);
	}
	if (!folder->exists(CONFIG_DIR))
	{
		folder->mkdir(CONFIG_DIR);
	}
	if (!folder->exists(IMG_DIR))
	{
		folder->mkdir(IMG_DIR);
	}

	QFile file(FILE_CFG);
	if (!file.open(QFile::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream in(&file);
	QString strData = in.readAll();
	QByteArray data = strData.toUtf8();
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
	if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
		if (doucment.isObject()) { // JSON 文档为对象
			QJsonObject object = doucment.object();  // 转化为对象
			//m_strWindowName = getString(object, QString("windowName"));
			m_strURL = getString(object, QString("url"));
			m_strPMSType = getString(object, QString("PMSType"));
			m_strGroupCode = getString(object, QString("GroupCode"));
			m_strHotelCode = getString(object, QString("HotelCode"));
			m_strDeviceID = getString(object, QString("DeviceID"));
			m_strExtraData = getString(object, QString("ExtraData"));

			m_strLocalURLTocken = getString(object, QString("urlTocken"));
			m_strLocalURLCard = getString(object, QString("urlCard"));
			m_strChainNum = getString(object, QString("chainNum"));
			m_strLockNum = getString(object, QString("lockNum"));
			m_strKey = getString(object, QString("key"));
			m_strHotelID = getString(object, QString("hotelID"));
			m_strDeviecAlias = getString(object, QString("deviceAlias"));
			m_strUser = getString(object, QString("user"));
			m_strPWD = getString(object, QString("pwd"));
		}
	}

	file.close();
}

void Configure::initWindowName()
{
	QFile file(FILE_WINDOW_NAME);
	if (!file.open(QFile::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream in(&file);
	m_strWindowName = in.readAll();
	file.close();
}

QString Configure::getString(QJsonObject &object, const QString& key)
{
	if (object.contains(key)) {  
		QJsonValue value = object.value(key); 
		if (value.isString()) { 
			return value.toString(); 
		}
	}

	return QString("");
}

int Configure::getInt(QJsonObject& object, const QString& key)
{
	if (object.contains(key)) {
		QJsonValue value = object.value(key);
		if (value.isDouble()) {
			return value.toInt();
		}
	}

	return 0;
}

QJsonObject Configure::getObject(QJsonObject& object, const QString& key)
{
	if (object.contains(key)) {
		QJsonValue value = object.value(key);
		if (value.isObject()) {
			return value.toObject();
		}
	}

	return QJsonObject();
}

void Configure::save()
{
	QFile file(FILE_CFG);
	if (!file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
	{
		return ;
	}

	QTextStream out(&file);
	
	QJsonObject json;
	//json.insert("windowName", m_strWindowName);
	json.insert("url", m_strURL);
	json.insert("PMSType", m_strPMSType);
	json.insert("GroupCode", m_strGroupCode);
	json.insert("HotelCode", m_strHotelCode);
	json.insert("DeviceID", m_strDeviceID);
	json.insert("ExtraData", m_strExtraData);

	json.insert("urlTocken",m_strLocalURLTocken);
	json.insert("urlCard",m_strLocalURLCard);
	json.insert("chainNum",m_strChainNum);
	json.insert("lockNum",m_strLockNum);
	json.insert("key",m_strKey);
	json.insert("hotelID",m_strHotelID);
	json.insert("deviceAlias",m_strDeviecAlias);
	json.insert("user",m_strUser);
	json.insert("pwd",m_strPWD);

	QJsonDocument document;
	document.setObject(json);
	document.toJson(QJsonDocument::Compact);
	
	out << document.toJson(QJsonDocument::Indented);;

	file.close();
}

QString Configure::getWindowName()
{
	return m_strWindowName;
}

void Configure::setUrl(QString& url)
{
	m_strURL = url;
	save();
}
QString Configure::getUrl()
{
	return m_strURL;
}

void Configure::setPMSType(QString& strPMSType)
{
	m_strPMSType = strPMSType;
	save();
}
QString Configure::getPMSType()
{
	return m_strPMSType;
}

void Configure::setGroupCode(QString& strGroupCode)
{
	m_strGroupCode = strGroupCode;
	save();
}
QString Configure::getGroupCode()
{
	return m_strGroupCode;
}

void Configure::setHotelCode(QString& strHotelCode)
{
	m_strHotelCode = strHotelCode;
	save();
}
QString Configure::getHotelCode()
{
	return m_strHotelCode;
}

void Configure::setDeviceID(QString& strDeviceID)
{
	m_strDeviceID = strDeviceID;
	save();
}
QString Configure::getDeviceID()
{
	return m_strDeviceID;
}

void Configure::setExtraData(QString& strExtraData)
{
	m_strExtraData = strExtraData;
	//save();
}
QString Configure::getExtraData()
{
	return m_strExtraData;
}
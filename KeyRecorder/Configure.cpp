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
}

void Configure::init()
{
	QDir* folder = new QDir;
	bool exist = folder->exists(CONFIG_DIR);
	if (!exist)
	{
		folder->mkdir(CONFIG_DIR);
		folder->mkdir(IMG_DIR);
	}

	QFile file(FILE_CFG);
	if (!file.open(QFile::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream in(&file);
	//QByteArray data;
	//in >> data;
	QString strData = in.readAll();
	QByteArray data = strData.toUtf8();
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
	if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
		if (doucment.isObject()) { // JSON 文档为对象
			QJsonObject object = doucment.object();  // 转化为对象
			m_strWindowName = getString(object, QString("windowName"));
			m_strURL = getString(object, QString("url"));
			m_strPMSType = getString(object, QString("PMSType"));
			m_strGroupCode = getString(object, QString("GroupCode"));
			m_strHotelCode = getString(object, QString("HotelCode"));
			m_strDeviceID = getString(object, QString("DeviceID"));
			m_strExtraData = getString(object, QString("ExtraData"));
		}
	}

	file.close();
}

QString Configure::getString(QJsonObject &object, QString& key)
{
	if (object.contains(key)) {  
		QJsonValue value = object.value(key); 
		if (value.isString()) { 
			return value.toString(); 
		}
	}

	return QString("");
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
	json.insert("windowName", m_strWindowName);
	json.insert("url", m_strURL);
	json.insert("PMSType", m_strPMSType);
	json.insert("GroupCode", m_strGroupCode);
	json.insert("HotelCode", m_strHotelCode);
	json.insert("DeviceID", m_strDeviceID);
	json.insert("ExtraData", m_strExtraData);

	QJsonDocument document;
	document.setObject(json);
	document.toJson(QJsonDocument::Compact);
	
	out << document.toJson(QJsonDocument::Indented);;

	file.close();
}

void Configure::setWindowName(QString& name)
{
	m_strWindowName = name;
	save();
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
	save();
}
QString Configure::getExtraData()
{
	return m_strExtraData;
}
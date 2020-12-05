#include "Configure.h"
#include <QFile>
#include "constant.h"
#include <QTextStream>
#include <QJsonDocument>
#include <QByteArray>


Configure::Configure()
{
	init();
}

void Configure::init()
{
	QFile file(FILE_CFG);
	if (!file.open(QFile::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream in(&file);
	QString strData = in.readAll();
	QByteArray data = strData.toLatin1();
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // ת��Ϊ JSON �ĵ�
	if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // ����δ��������
		if (doucment.isObject()) { // JSON �ĵ�Ϊ����
			QJsonObject object = doucment.object();  // ת��Ϊ����
			m_strURL = getString(object, QString("url"));
			m_strPMSType = getString(object, QString("PMSType"));
			m_strGroupCode = getString(object, QString("GroupCode"));
			m_strHotelCode = getString(object, QString("HotelCode"));
			m_strDeviceID = getString(object, QString("DeviceID"));
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
	json.insert("url", m_strURL);
	json.insert("PMSType", m_strPMSType);
	json.insert("GroupCode", m_strGroupCode);
	json.insert("HotelCode", m_strHotelCode);
	json.insert("DeviceID", m_strDeviceID);

	QJsonDocument document;
	document.setObject(json);
	document.toJson(QJsonDocument::Compact);
	
	out << document.toJson(QJsonDocument::Indented);;

	file.close();
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
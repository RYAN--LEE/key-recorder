#pragma once
#include <QString>
#include <QJsonObject>

class Configure
{
public:
	static Configure* instance()
	{
		static Configure instance;
		return &instance;
	}

public:
	void setWindowName(QString& name);
	QString getWindowName();

	void setUrl(QString& url);
	QString getUrl();

	void setPMSType(QString& strPMSType);
	QString getPMSType();

	void setGroupCode(QString& strGroupCode);
	QString getGroupCode();

	void setHotelCode(QString& strHotelCode);
	QString getHotelCode();

	void setDeviceID(QString& strDeviceID);
	QString getDeviceID();

	static QString getString(QJsonObject& object, QString &key);

private:
	Configure();
	void init();
	void save();

private:
	static Configure* m_instance;

	QString m_strWindowName;

	QString m_strURL;
	QString m_strPMSType;
	QString m_strGroupCode;
	QString m_strHotelCode;
	QString m_strDeviceID;

};


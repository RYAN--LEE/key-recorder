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

	void setExtraData(QString& strExtraData);
	QString getExtraData();

	static QString getString(QJsonObject& object, const QString& key);
	static int getInt(QJsonObject& object, const QString &key);
	static QJsonObject getObject(QJsonObject& object, const QString& key);

private:
	Configure();
	void init();
	void save();

	void initWindowName();

private:
	static Configure* m_instance;

	QString m_strWindowName;

	QString m_strURL;
	QString m_strPMSType;
	QString m_strGroupCode;
	QString m_strHotelCode;
	QString m_strDeviceID;
	QString m_strExtraData;

public:
	QString m_strLocalURLTocken;
	QString m_strLocalURLCard;
	QString m_strChainNum;
	QString m_strLockNum;
	QString m_strKey;
	QString m_strHotelID;
	QString m_strDeviecAlias;
	QString m_strUser;
	QString m_strPWD;
};


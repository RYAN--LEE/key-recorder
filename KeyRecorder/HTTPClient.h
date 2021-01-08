#pragma once
#include <QObject>
#include <QNetworkReply>
#include <QSslError>

class HTTPClient : public QObject
{
	Q_OBJECT
public:
	QString getRoomNumber(QString& name, QString& cardID, QString& roomNum);
	QString createCard(QString& name, QString& cardID, QString roomNum);

public slots:
	void replyFinished(QNetworkReply* reply);

private:
	QString sendRequest(QString& url, QByteArray& qByteHttpData, QJsonObject& retJsonObject);

private:
	QByteArray makeRoomRequestJson(QString& name, QString& cardID);
	QByteArray makeCardRequestJson(QString& name, QString& cardID, QString& roomNum);
};


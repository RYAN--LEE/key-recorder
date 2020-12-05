#pragma once
#include <QObject>
#include <QNetworkReply>
#include <QSslError>

class HTTPClient : public QObject
{
	Q_OBJECT
public:
	QString getRoomNumber(QString& name, QString& cardID, QString& roomNum);

public slots:
	void replyFinished(QNetworkReply* reply);

private:
	QByteArray makeRequestJson(QString& name, QString& cardID);
};


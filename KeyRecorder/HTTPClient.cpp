#include "HTTPClient.h"
#include "Configure.h"
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QEventLoop>


QString HTTPClient::getRoomNumber(QString& name, QString& cardID, QString &roomNum)
{
    Configure* cfg = Configure::instance();
    QNetworkAccessManager* pHttpMgr = new QNetworkAccessManager(this);
    //����url
    QString url = cfg->getUrl();
    //����ͷ��Ϣ
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    //  requestInfo.setRawHeader("Content-Type","application/json");//������Ҫ�������ͷ��
    //  requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//������Ҫ�������ͷ��

        //��������
    QByteArray qByteHttpData = makeRequestJson(name, cardID);
    QNetworkReply* reply = pHttpMgr->post(requestInfo, qByteHttpData);
    
    //connect(pHttpMgr, &QNetworkAccessManager::finished, this, &HTTPClient::replyFinished);

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    //������
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "request protobufHttp handle errors here";
        return reply->errorString();
    }

    QByteArray data = reply->readAll();
    qDebug() << "room info " << data;


    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // ת��Ϊ JSON �ĵ�
    if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError)) {  // ����δ��������
        return "json parse error";
    }
    if (!doucment.isObject()) { // JSON �ĵ�Ϊ����
        return "json parse object error";
    }
    QJsonObject object = doucment.object();  // ת��Ϊ����
    QString result = Configure::getString(object, QString("result"));
    roomNum = Configure::getString(object, QString("roomNum"));
    return result;
}


void HTTPClient::replyFinished(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // ת��Ϊ JSON �ĵ�
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // ����δ��������
        if (doucment.isObject()) { // JSON �ĵ�Ϊ����
            QJsonObject object = doucment.object();  // ת��Ϊ����
            QString Result = Configure::getString(object, QString("Result"));
            QString RoomNum = Configure::getString(object, QString("RoomNum"));
        }
    }
}

QByteArray HTTPClient::makeRequestJson(QString &name, QString& cardID)
{
    Configure* cfg = Configure::instance();
    QJsonObject json;
    json.insert("Type", "001");
    json.insert("Index", 1);
    json.insert("PMSType", cfg->getPMSType());
    json.insert("GroupCode", cfg->getGroupCode());
    json.insert("Name", name);
    json.insert("CardID", cardID);
    json.insert("DeviceID", cfg->getDeviceID());
    json.insert("SendTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    QJsonDocument document;
    document.setObject(json);
    return document.toJson(QJsonDocument::Indented);
}

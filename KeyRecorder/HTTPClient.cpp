#include "HTTPClient.h"
#include "Configure.h"
#include "constant.h"
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QEventLoop>
#include <QTimer>


QString HTTPClient::getRoomNumber(QString& name, QString& cardID, QString &roomNum)
{
    QByteArray qByteHttpData = makeRoomRequestJson(name, cardID);
    QJsonObject object;
    Configure* cfg = Configure::instance();
    QString url = cfg->getUrl() + "/" + API_GET_ROOM_NUM;

    QString ret = sendRequest(url, qByteHttpData, object);
    if (ret != "ok")
    {
        return ret;
    }

    QString result = Configure::getString(object, QString("Result"));
    roomNum = Configure::getString(object, QString("RoomNum"));
    return result;
}

QString HTTPClient::createCard(QString& name, QString& cardID, QString roomNum)
{
    QByteArray qByteHttpData = makeCardRequestJson(name, cardID, roomNum);
    QJsonObject object;

    Configure* cfg = Configure::instance();
    QString url = cfg->getUrl() + "/" + API_CREATE_CARD;

    QString ret = sendRequest(url, qByteHttpData, object);
    if (ret != "ok")
    {
        return ret;
    }

    QString result = Configure::getString(object, QString("Result"));
    return result;
}

QString HTTPClient::sendRequest(QString &url, QByteArray & qByteHttpData, QJsonObject& retJsonObject)
{
    QNetworkAccessManager* pHttpMgr = new QNetworkAccessManager(this);
    //����ͷ��Ϣ
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    //  requestInfo.setRawHeader("Content-Type","application/json");//������Ҫ�������ͷ��
    //  requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//������Ҫ�������ͷ��

    //��������
    QNetworkReply* reply = pHttpMgr->post(requestInfo, qByteHttpData);

    //connect(pHttpMgr, &QNetworkAccessManager::finished, this, &HTTPClient::replyFinished);

    QTimer timer;
    timer.setInterval(3000);
    timer.setSingleShot(true);
    timer.start();

    QEventLoop eventLoop;
    connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    if (!timer.isActive()) {
        return "time out!";
    }
    timer.stop();
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
    retJsonObject = doucment.object();  // ת��Ϊ����
    return "ok";
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

QByteArray HTTPClient::makeRoomRequestJson(QString &name, QString& cardID)
{
    Configure* cfg = Configure::instance();
    QJsonObject json;
    json.insert("Type", "P01");
    json.insert("Index", 1);
    json.insert("PMSType", cfg->getPMSType());
    json.insert("GroupCode", cfg->getGroupCode());
    json.insert("HotelCode", cfg->getHotelCode());
    json.insert("Name", name);
    json.insert("CardID", cardID);
    json.insert("DeviceID", cfg->getDeviceID());
    json.insert("SendTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    QJsonDocument document;
    document.setObject(json);
    return document.toJson(QJsonDocument::Indented);
}

QByteArray HTTPClient::makeCardRequestJson(QString& name, QString& cardID, QString& roomNum)
{
    Configure* cfg = Configure::instance();
    QJsonObject json;
    json.insert("Type", "P03");
    json.insert("Index", 1);
    json.insert("PMSType", cfg->getPMSType());
    json.insert("GroupCode", cfg->getGroupCode());
    json.insert("Name", name);
    json.insert("CardID", cardID);
    json.insert("RoomNum", roomNum);
    json.insert("DeviceID", cfg->getDeviceID());
    json.insert("SendTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    QJsonObject extraJson;
    extraJson.insert("Account", cfg->getExtraData());

    json.insert("ExtData", extraJson);

    QJsonDocument document;
    document.setObject(json);
    return document.toJson(QJsonDocument::Indented);
}

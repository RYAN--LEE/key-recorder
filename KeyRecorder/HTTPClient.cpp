#include "HTTPClient.h"
#include "Configure.h"
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QEventLoop>


QString HTTPClient::getRoomNumber(QString& name, QString& cardID, QString &roomNum)
{
    Configure* cfg = Configure::instance();
    QNetworkAccessManager* pHttpMgr = new QNetworkAccessManager(this);
    //设置url
    QString url = cfg->getUrl();
    //设置头信息
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    //  requestInfo.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
    //  requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部

        //发送数据
    QByteArray qByteHttpData = makeRequestJson(name, cardID);
    QNetworkReply* reply = pHttpMgr->post(requestInfo, qByteHttpData);
    
    //connect(pHttpMgr, &QNetworkAccessManager::finished, this, &HTTPClient::replyFinished);

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    //错误处理
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "request protobufHttp handle errors here";
        return reply->errorString();
    }

    QByteArray data = reply->readAll();
    qDebug() << "room info " << data;


    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
    if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError)) {  // 解析未发生错误
        return "json parse error";
    }
    if (!doucment.isObject()) { // JSON 文档为对象
        return "json parse object error";
    }
    QJsonObject object = doucment.object();  // 转化为对象
    QString result = Configure::getString(object, QString("result"));
    roomNum = Configure::getString(object, QString("roomNum"));
    return result;
}


void HTTPClient::replyFinished(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
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

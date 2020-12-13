#include "HttpServer.h"
//#include "JQLibrary\include\JQHttpServer"
#include <QPointer>
#include <QDebug>

//void onHttpAccepted(const QPointer< JQHttpServer::Session >& session)
//{
//    // 回调发生在专用的处理线程内，不是主线程，也不是socket的生存线程，请注意线程安全
//    // 若阻塞了此回调，那么新的连接将被阻塞（默认情况下有2个线程可以阻塞2次，第3个连接将被阻塞）
//
//    session->replyText(QString("url:%1\nbody:%2").arg(session->requestUrl(), QString(session->requestBody())));
//    //    session->replyRedirects( QUrl( "http://www.baidu.com" ) );
//    //    session->replyJsonObject( { { { "message", "ok" } } } );
//    //    session->replyJsonArray( { "a", "b", "c" } );
//    //    session->replyFile( "/Users/jason/Desktop/Test1.Test2" );
//    //    session->replyImage( QImage( "/Users/jason/Desktop/Test.png" ) );
//    //    session->replyBytes( QByteArray( 4,'\x24' ), "text" ); // $$$$
//
//        // 注1：因为一个session对应一个单一的HTTP请求，所以session只能reply一次
//        // 注2：在reply后，session的生命周期不可控，所以reply后不要再调用session的接口了
//}
//
//void HttpServer::start()
//{
//    static JQHttpServer::TcpServerManage tcpServerManage(2); // 设置最大处理线程数，默认2个
//    tcpServerManage.setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1));
//
//    const auto listenSucceed = tcpServerManage.listen(QHostAddress::Any, 8080);
//    qDebug() << "HTTP server listen:" << listenSucceed;
//
//}

#include "HttpServer.h"
//#include "JQLibrary\include\JQHttpServer"
#include <QPointer>
#include <QDebug>

//void onHttpAccepted(const QPointer< JQHttpServer::Session >& session)
//{
//    // �ص�������ר�õĴ����߳��ڣ��������̣߳�Ҳ����socket�������̣߳���ע���̰߳�ȫ
//    // �������˴˻ص�����ô�µ����ӽ���������Ĭ���������2���߳̿�������2�Σ���3�����ӽ���������
//
//    session->replyText(QString("url:%1\nbody:%2").arg(session->requestUrl(), QString(session->requestBody())));
//    //    session->replyRedirects( QUrl( "http://www.baidu.com" ) );
//    //    session->replyJsonObject( { { { "message", "ok" } } } );
//    //    session->replyJsonArray( { "a", "b", "c" } );
//    //    session->replyFile( "/Users/jason/Desktop/Test1.Test2" );
//    //    session->replyImage( QImage( "/Users/jason/Desktop/Test.png" ) );
//    //    session->replyBytes( QByteArray( 4,'\x24' ), "text" ); // $$$$
//
//        // ע1����Ϊһ��session��Ӧһ����һ��HTTP��������sessionֻ��replyһ��
//        // ע2����reply��session���������ڲ��ɿأ�����reply��Ҫ�ٵ���session�Ľӿ���
//}
//
//void HttpServer::start()
//{
//    static JQHttpServer::TcpServerManage tcpServerManage(2); // ����������߳�����Ĭ��2��
//    tcpServerManage.setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1));
//
//    const auto listenSucceed = tcpServerManage.listen(QHostAddress::Any, 8080);
//    qDebug() << "HTTP server listen:" << listenSucceed;
//
//}

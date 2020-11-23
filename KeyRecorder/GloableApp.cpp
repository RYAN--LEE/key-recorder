#include "GloableApp.h"
#include <QDebug>
#include <QCursor>

GloableApp::GloableApp(int& argc, char** argv) 
    :QApplication(argc, argv)
{

}
bool GloableApp::notify(QObject* obj, QEvent* e)
{

    if (e->type() == QEvent::KeyPress || e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseMove)
    {
        //qDebug() << e->type();

        QPoint pt = QCursor::pos();
        //qDebug() << "x:" << pt.x() << "y:" << pt.y();
    }
    return QApplication::notify(obj, e);
}

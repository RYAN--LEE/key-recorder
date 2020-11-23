#include "KeyRecorder.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QThread>
#include "constant.h"


KeyRecorder::KeyRecorder(QWidget *parent)
    : QMainWindow(parent)
    , m_nPlayInterval(200)
{
    ui.setupUi(this);
    m_pMouseHook = MouseHook::instance();

    m_pFormMoreInfo = new FormMoreInfo(this);
    m_pFormMoreInfo->hide();

    connect(m_pMouseHook, &MouseHook::mouseClicked, this, &KeyRecorder::recieveClicked);
    connect(this, &KeyRecorder::startPlay, this, &KeyRecorder::playClick);
}

void KeyRecorder::recieveClicked(long x, long y)
{
    ui.label->setText(QString("x:") + QString::number(x) + QString(" y:") + QString::number(y));

    m_vecPoint.push_back(QPoint(x, y));
}

void KeyRecorder::on_pushButtonStart_clicked()
{
    m_pMouseHook->setHook();
}

void KeyRecorder::on_pushButtonStop_clicked()
{
    m_pMouseHook->unHook();

    save(m_vecPoint, 0, m_vecPoint.size()-1);
    m_vecPoint.clear();
}

void KeyRecorder::on_pushButtonMore_clicked()
{
    m_pFormMoreInfo->show();
}

void KeyRecorder::on_pushButtonPlay_clicked()
{
    QFile file(SAVE_FILE, this);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString data = in.readAll();
    file.close();

    QStringList points = data.split('\n');

    QVector<QPoint> vecPoint;

    for (QList<QString>::iterator itr = points.begin(); itr != points.end(); itr++)
    {
        QStringList p = itr->split(',');
        if (p.size() >= 2)
        {
            QPoint point(p[0].toLong(), p[1].toLong());
            vecPoint.push_back(point);
        }
    }

    emit startPlay(vecPoint);
}

void KeyRecorder::playClick(QVector<QPoint> points)
{
    qDebug() << "playClick start" << endl;
    for (int i = 0; i < points.size(); i++)
    {
        qDebug() << "playClick " << points[i] << endl;
        m_pMouseHook->clickKey(points[i].x(), points[i].y());
        QThread::msleep(m_nPlayInterval);
    }
    qDebug() << "playClick end" << endl;
}

bool KeyRecorder::save(QVector<QPoint>& vecPoint, int begin, int end)
{
    if (end < begin)
    {
        return false;
    }

    QFile file(SAVE_FILE, this);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
    {
        qDebug() << "open failed";
        return false;
    }

    QTextStream out(&file);
    for (int i = 0; i < end; i++)
    {
        out << vecPoint[i].x() << "," << vecPoint[i].y() << endl;
    }
    out.flush();

    file.close();
    return true;
}
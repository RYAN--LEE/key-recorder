#include "KeyRecorder.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QByteArray>
#include "constant.h"
#include "ImageMacher.h"
#include "KeyInfo.h"


KeyRecorder::KeyRecorder(QWidget *parent)
    : QMainWindow(parent)
    , m_nPlayInterval(200)
    , m_bRecord(false)
    //, m_bPlay(false)
{
    ui.setupUi(this);
    m_pMouseHook = MouseHook::instance();

    m_pTaskThread = new TaskThread(this, m_pMouseHook);
    m_pTaskThread->start();

    m_pKeyStore = new KeyStore(this);

    m_pFormMoreInfo = new FormMoreInfo(this);
    m_pFormMoreInfo->hide();

    m_pScreenGraber = new ScreenGraber(this);
    m_pRecongnizer = new Recongnizer();

    connect(m_pMouseHook, &MouseHook::mouseClicked, this, &KeyRecorder::recieveClicked);
    connect(this, &KeyRecorder::startPlay, this, &KeyRecorder::playClick);
}

void KeyRecorder::recieveClicked(long x, long y)
{
    ui.label->setText(QString("x:") + QString::number(x) + QString(" y:") + QString::number(y));

    if (m_bRecord)
    {
        m_recordBegin = m_recordEnd;
        m_recordEnd = m_recordPos;
        m_recordPos = QPoint(x, y); 
    }
    else {
        QTime current = QTime::currentTime();
        int interval = 0;
        if (!m_LastClickTime.isNull())
        {
            interval = m_LastClickTime.msecsTo(current);
        }

        m_vecPoint.push_back(KeyInfo(x, y, interval));
        m_LastClickTime = current;
    }
}

void KeyRecorder::on_pushButtonStart_clicked()
{
    m_pMouseHook->setHook();
}

void KeyRecorder::on_pushButtonStop_clicked()
{
    m_pMouseHook->unHook();

    if (m_vecPoint.size() > 0)
    {
        // 去掉最后一次点击结束
        m_vecPoint.pop_back();
    }

    m_pKeyStore->saveKeys(m_vecPoint);

    //save(m_vecPoint, 0, m_vecPoint.size()-1);
    m_vecPoint.clear();
    m_LastClickTime = QTime();
}

void KeyRecorder::on_pushButtonMore_clicked()
{
    m_pFormMoreInfo->show();
}

void KeyRecorder::on_pushButtonStartRcd_clicked()
{
    m_pMouseHook->setHook();
    m_bRecord = true;
}

void KeyRecorder::on_pushButtonStopRcd_clicked()
{
    m_bRecord = false;
    m_pMouseHook->unHook();
}

void KeyRecorder::on_pushButtonTest_clicked()
{
    QRect rect(m_recordBegin, m_recordEnd);
    qDebug() << "record rect:" << rect;
    QByteArray bytes;
    m_pScreenGraber->grabScreen(rect.x(), rect.y(), rect.width(), rect.height(), bytes);

    bool ok = false;
    int nThreshold = ui.lineEditThreshold->text().toInt(&ok);
    if (!ok){
        nThreshold = 100;
    }

    QByteArray grayBytes;
    ImageMacher macher;
    macher.grayImage(bytes, grayBytes, nThreshold);
    //macher.enhanceImage();

    QString strResult = m_pRecongnizer->DoRecongnize(grayBytes.data(), grayBytes.count());

    ui.labelResult->setText(strResult);
}

bool KeyRecorder::checkScreenStatus(int status)
{
    QString strTemplate = getTemplate(status);
    if (strTemplate.isEmpty())
    {
        return false;
    }

    QByteArray bytes;
    m_pScreenGraber->grabScreen(0, 0, -1, -1, bytes);
    ImageMacher mach;
    return mach.matchImage(bytes, strTemplate);
}

QString KeyRecorder::getTemplate(int status)
{
    return "";
}


void KeyRecorder::on_pushButtonPlay_clicked()
{
    QVector<KeyInfo> vecPoint = m_pKeyStore->getKeys();
    emit startPlay(vecPoint);
}

void KeyRecorder::on_pushButtonPause_clicked()
{
    m_pTaskThread->pause();
    //m_bPlay = false;
}

void KeyRecorder::playClick(QVector<KeyInfo> points)
{
    m_pTaskThread->setKeyInfos(points);
    m_pTaskThread->play();
    /*m_bPlay = true;
    qDebug() << "playClick start" << endl;
    for (int i = 0; i < points.size(); i++)
    {
        if (!m_bPlay)
        {
            return;
        }
        qDebug() << "playClick " << points[i].string() << endl;
        if (points[i].interval() > 0)
        {
            QThread::msleep(points[i].interval());
        }
        m_pMouseHook->clickKey(points[i].x(), points[i].y());
    }

    m_bPlay = false;
    qDebug() << "playClick end" << endl;*/
}

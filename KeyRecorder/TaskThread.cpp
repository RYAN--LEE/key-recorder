#include "TaskThread.h"
#include <QDebug>

TaskThread::TaskThread(QObject *parent, MouseHook* pMouseHook)
	: QThread(parent)
	, m_bPlay(false)
    , m_pMouseHook(pMouseHook)
{
}

TaskThread::~TaskThread()
{
}

void TaskThread::run()
{
	while (isRunning())
	{
		if (!m_bPlay)
		{
			msleep(500);
			continue;
		}

        m_mutex.lock();
		QVector<KeyInfo> vecInfo = m_vecKeyInfo;
		m_mutex.unlock();

		for (int i = 0; i < vecInfo.size(); i++)
        {
            if (!m_bPlay)
            {
                break;
            }
            KeyInfo keyInfo = m_vecKeyInfo[i];
            qDebug() << "playClick " << keyInfo.string() << endl;
            if (keyInfo.interval() > 0)
            {
                QThread::msleep(keyInfo.interval());
            }
            if (!keyInfo.condition().isEmpty())
            {
                while (!handleCondition(keyInfo.condition()))
                {
                    QThread::msleep(1000);
                }
            }
            m_pMouseHook->clickKey(keyInfo.x(), keyInfo.y());
        }
	}
}

bool TaskThread::handleCondition(QString& condition)
{
    //todo
    return true;
}

void TaskThread::play()
{
    m_bPlay = true;
}
void TaskThread::pause()
{
    m_bPlay = false;
}

void TaskThread::setKeyInfos(QVector<KeyInfo> vecKeyInfo)
{
    m_mutex.lock();
    m_vecKeyInfo = vecKeyInfo;
    m_mutex.unlock();
}
#include "TaskThread.h"
#include "ImageMacher.h"
#include "Utils.h"
#include "constant.h"
#include "HTTPClient.h"
#include <QDebug>
#include <QRect>
#include <QClipboard>
#include <QApplication>

TaskThread::TaskThread(QObject *parent, MouseHook* pMouseHook, Recongnizer* pRecongnizer, ScreenGraber* pScreenGraber)
	: QThread(parent)
	, m_bPlay(false)
    , m_pMouseHook(pMouseHook)
    , m_pRecongnizer(pRecongnizer)
    , m_pScreenGraber(pScreenGraber)
{
}

TaskThread::~TaskThread()
{
}

void pressKeyPaste()
{
    //setHookKey();
//keybd_event(VK_CONTROL, 0, 0, 0);
   // keybd_event('v', 0, 0, 0);

    //keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    //keybd_event('v', 0, KEYEVENTF_KEYUP, 0);
    //unHookKey();

    INPUT input[6];
    memset(input, 0, 6 * sizeof(INPUT));
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = '1';

    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wVk = '1';
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    input[2].type = INPUT_KEYBOARD;
    input[2].ki.wVk = '2';

    input[3].type = INPUT_KEYBOARD;
    input[3].ki.wVk = '2';
    input[3].ki.dwFlags = KEYEVENTF_KEYUP;

    input[4].type = INPUT_KEYBOARD;
    input[4].ki.wVk = '3';

    input[5].type = INPUT_KEYBOARD;
    input[5].ki.wVk = '3';
    input[5].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(6, input, sizeof(INPUT));

    qDebug() << "press key" << endl;
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
        m_strName = "";
        m_strID = "";

        m_mutex.lock();
        QMap<int, KeyInfo> keyMap;
        for (int i = 0; i < m_vecKeyInfo.size(); ++i)
        {
            KeyInfo info = m_vecKeyInfo[i];
            keyMap[info.m_id] = info;
        }
		m_mutex.unlock();


        KeyInfo next = m_vecKeyInfo[0];
		while (next.m_nextID != 0 || next.m_breakID != 0)
        {
            if (!m_bPlay)
            {
                break;
            }
            qDebug() << "playClick " << next.string() << endl;
            if (next.interval() > 0)
            {
                QThread::msleep(next.interval());
            }

            int x = next.x();
            int y = next.y();
            adjustPoint(next.m_adjustRect, x, y);
            m_pMouseHook->clickKey(x, y);

            if (!next.condition().isEmpty())
            {
                bool bRet = handleCondition(next.condition(), next.m_adjustRect);
                int nextID = bRet ? next.m_nextID : next.m_breakID;
                if (!keyMap.contains(nextID))
                {
                    break;
                }
                next = keyMap[nextID];
                continue;
            }

            if (next.m_nextID == 0)
            {
                break;
            }

            if (!keyMap.contains(next.m_nextID))
            {
                break;
            }
            next = keyMap[next.m_nextID];
            //QThread::msleep(1000);
            //inputData(QString("qwe"));
        }

        QThread::msleep(1000);

	}
}

void TaskThread::adjustPoint(QRect adjustRect, int &x, int &y)
{

    
}

bool TaskThread::handleCondition(QString& condition, QRect &ajustRect)
{
    if (condition.contains("img_"))
    {
        //handle img match
        QRect matchRect;
        bool bRet = checkScreenStatus(condition, matchRect);
        if (bRet)
        {
            ajustRect = matchRect;
        }
        imageMatched(condition, bRet);
        return bRet;
    }
    else if (condition.contains("text_"))
    {
        m_strID = recongnizeText(condition);
        emit recongnizeValue(m_strID);

    }
    else if (condition.contains("room_"))
    {
        QThread::msleep(200);

        QString roomNo = getRoomNum(m_strName, m_strID);
        inputData(roomNo);
        emit roomInputed(roomNo);
    }
    return true;
}

QString TaskThread::recongnizeText(QString& imgDir)
{
    //handle text match
    QRect rect = Utils::decodeTextMatch(imgDir);
    if (!rect.isEmpty())
    {
        QByteArray bytes;
        m_pScreenGraber->grabScreen(rect.x(), rect.y(), rect.width(), rect.height(), bytes);

        int nThreshold = 100;

        QByteArray grayBytes;
        ImageMacher macher;
        macher.grayImage(bytes, grayBytes, nThreshold);
        //macher.enhanceImage();

        QString strResult = m_pRecongnizer->DoRecongnize(grayBytes.data(), grayBytes.count());

        strResult = strResult.replace(" ", "");
        strResult = strResult.simplified();
        return strResult;
    }
    return QString("");
}

QString TaskThread::getRoomNum(QString strName, QString strID)
{
    HTTPClient client;
    QString roomNum;
    QString strRet = client.getRoomNumber(strName, strID, roomNum);
    return roomNum;
}

QString TaskThread::getTemplate(QString &status)
{
    return IMG_DIR + status;
}


bool TaskThread::checkScreenStatus(QString &status, QRect& matchRect)
{
    QString strTemplate = getTemplate(status);
    if (strTemplate.isEmpty())
    {
        return false;
    }

    QByteArray bytes;
    m_pScreenGraber->grabScreen(0, 0, -1, -1, bytes);
    ImageMacher mach;
    return mach.matchImage(bytes, strTemplate, matchRect);
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

void TaskThread::inputData(QString& data)
{
    data = data.toUpper();

    /*wchar_t str[256] = { 0 };
    int nLen = data.toWCharArray(str);
    //int nLen = data.size();
    INPUT input[128];
    memset(input, 0, 2 * sizeof(INPUT));
    
    for (int i = 0; i < nLen; i++)
    {
        WORD vk = str[i];
        int index = i * 2;
        input[index].type = INPUT_KEYBOARD;
        input[index].ki.wVk = vk;

        index += 1;
        input[index].type = INPUT_KEYBOARD;
        input[index].ki.wVk = vk;
        input[index].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    
    SendInput(nLen*2, input, sizeof(INPUT));*/

    wchar_t str[256] = { 0 };
    int nLen = data.toWCharArray(str);
    for (int i = 0; i < nLen; i++)
    {
        INPUT input1;
        WORD vk = str[i];
        input1.type = INPUT_KEYBOARD;
        input1.ki.wVk = vk;
        SendInput(1, &input1, sizeof(INPUT));

        INPUT input2;
        input2.type = INPUT_KEYBOARD;
        input2.ki.wVk = vk;
        input2.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input2, sizeof(INPUT));
    }
}

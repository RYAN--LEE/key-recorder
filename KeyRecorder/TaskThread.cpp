#include "TaskThread.h"
#include "ImageMacher.h"
#include "Utils.h"
#include "constant.h"
#include "HTTPClient.h"
#include "Configure.h"
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


void TaskThread::maxmizeWindow()
{
    QString strName = Configure::instance()->getWindowName();
    // strName = name.toLocal8Bit().data();
    LPCWSTR wstrName;
    if (strName.isEmpty())
    {
        wstrName = L"旅馆业治安管理信息系统 版本号：20200501 识读核心：通用 主程序修改时间：2018-12-18 09:25:32";
    }
    else
    {
        wstrName = strName.toStdWString().c_str();
    }
    HWND hWnd = FindWindow(NULL, wstrName);
    if (hWnd != NULL)
    {
        ::ShowWindow(hWnd, SW_MAXIMIZE);
        ::SetForegroundWindow(hWnd);
        ::BringWindowToTop(hWnd);
        ::FlashWindow(hWnd, TRUE);
    }
}
void TaskThread::run()
{
    maxmizeWindow();
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

        m_strRoomNum = "";
        bool isFirst = true;
        KeyInfo currStep = m_vecKeyInfo[0];
		while (currStep.m_nextID != 0 || currStep.m_breakID != 0)
        {
            if (!m_bPlay)
            {
                break;
            }

            if (currStep.m_id == 0)
            {
                maxmizeWindow();
            }
            qDebug() << "playClick " << currStep.string() << endl;
            if (isFirst)
            {
                int interval = currStep.interval();
                if (interval == 0)
                {
                    interval = 100;
                }
                QThread::msleep(interval);

                if (!m_bPlay)
                    break;
            }

            QRect matchRect(-1, -1, 0, 0);
            bool bRet = beforClick(currStep.beforeCondition(), matchRect);
            if (!bRet)
            {
                isFirst = false;
                emit stepStatusChange(currStep.m_id, false, QString::fromLocal8Bit("识别失败"));
                QThread::msleep(200);
                continue;
            }

            emit stepStatusChange(currStep.m_id, true, QString::fromLocal8Bit("识别成功"));
            isFirst = true;

            if (!m_bPlay)
                break;

            adjustPoint(currStep, currStep.m_adjustRect, matchRect);
            m_pMouseHook->clickKey(currStep.m_adjustX, currStep.m_adjustY);
            emit stepStatusChange(currStep.m_id, true, 
                QString::fromLocal8Bit("点击(") + QString::number(currStep.m_adjustX)+":"+ QString::number(currStep.m_adjustY) + ")");

            if (!currStep.condition().isEmpty())
            {
                QRect matchRect(-1,-1,0,0);
                bool bRet = handleCondition(currStep.condition(), matchRect);
                if (bRet)
                {
                    emit stepStatusChange(currStep.m_id, true, QString::fromLocal8Bit("执行成功"));
                }
                else
                {
                    emit stepStatusChange(currStep.m_id, false, QString::fromLocal8Bit("执行失败"));
                }
                int nextID = bRet ? currStep.m_nextID : currStep.m_breakID;
                if (!keyMap.contains(nextID))
                {
                    break;
                }
                QRect adjustRect = currStep.m_adjustRect;
                currStep = keyMap[nextID];

                continue;
            }

            if (currStep.m_nextID == 0)
            {
                break;
            }

            if (!keyMap.contains(currStep.m_nextID))
            {
                break;
            }
            currStep = keyMap[currStep.m_nextID];
            //QThread::msleep(1000);
            //inputData(QString("qwe"));
        }

        QThread::msleep(1000);

	}
}

void TaskThread::adjustPoint(KeyInfo &keyInfo, QRect adjustRect, QRect matchRect)
{
    if (matchRect.x() >= 0 && matchRect.y() >= 0)
    {
        keyInfo.m_adjustX = matchRect.x() - adjustRect.x() + keyInfo.x();
        keyInfo.m_adjustY = matchRect.y() - adjustRect.y() + keyInfo.y();
    }
}

bool TaskThread::beforClick(QString& condition, QRect& ajustRect)
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
        emit imageMatched(condition, bRet);
        return bRet;
    }

    return true;
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
        emit imageMatched(condition, bRet);
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
        if (roomNo.isEmpty()) 
        {
            emit showMsg(QString::fromLocal8Bit("错误"),
                QString::fromLocal8Bit("获取房间失败，身份证号：") + m_strID);
            emit roomInputed("failed");
            return false;
        }
        m_strRoomNum = roomNo;
        inputData(roomNo);
        emit roomInputed(roomNo);
    }
    else if (condition.contains("create_card"))
    {
        QThread::msleep(200);
        if (m_strRoomNum.isEmpty())
        {
            emit createCardFinish(false);
            return false;
        }

        bool bRet = makeCard(m_strName, m_strID, m_strRoomNum);
        emit createCardFinish(bRet);
        if (!bRet)
        {
            emit showMsg(QString::fromLocal8Bit("错误"),
                QString::fromLocal8Bit("制卡失败，身份证号：") + m_strID +
                QString::fromLocal8Bit(" 房间号：") + m_strRoomNum);
        }
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
    if (strRet == "Success")
    {
        return roomNum;
    }
    return "";
}

bool TaskThread::makeCard(QString strName, QString strID, QString roomNum)
{
    HTTPClient client;
    QString strRet = client.createCard(strName, strID, roomNum);
    if (strRet == "Success")
    {
        return true;
    }
    return false;
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
        memset(&input1, 0, sizeof(INPUT));
        WORD vk = str[i];
        input1.type = INPUT_KEYBOARD;
        input1.ki.wVk = vk;
        uint ret = SendInput(1, &input1, sizeof(INPUT));
        DWORD err = GetLastError();

        INPUT input2;
        memset(&input2, 0, sizeof(INPUT));
        input2.type = INPUT_KEYBOARD;
        input2.ki.wVk = vk;
        input2.ki.dwFlags = KEYEVENTF_KEYUP;
        ret = SendInput(1, &input2, sizeof(INPUT));
    }
}

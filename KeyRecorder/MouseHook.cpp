#include "MouseHook.h"
#include<QDebug>


//���̹��ӹ���
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //��WH_KEYBOARD_LLģʽ��lParam ��ָ��KBDLLHOOKSTRUCT���͵�ַ
    KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
    qDebug() << "key:" << pkbhs->vkCode;
    if (pkbhs->vkCode == VK_F10)
    {
        MouseHook::instance()->setKeyPause();
    }
    else if (pkbhs->vkCode == VK_F9)
    {
        MouseHook::instance()->setKeyStart();
    }
    return 0;//����1��ʾ��ȡ��Ϣ���ٴ���,����0��ʾ��������,��Ϣ��������

}
//��깳�ӹ���
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case WM_LBUTTONDOWN:
        qDebug() << "down";
        break;
    case WM_LBUTTONUP:
        qDebug() << "up";
        MSLLHOOKSTRUCT* pmshs = (MSLLHOOKSTRUCT*)lParam;
        MouseHook::instance()->setMouseClick(pmshs->pt.x, pmshs->pt.y);

        /*   mouse_event(WM_LBUTTONDOWN, 0, 10, 10, 0);
           mouse_event(WM_LBUTTONUP, 0, 0, 0, 0);*/

        break;
    }

   // MSLLHOOKSTRUCT* pmshs = (MSLLHOOKSTRUCT*)lParam;
   // qDebug() << "wParam:" << wParam << "x:" << pmshs->pt.x << "y:" << pmshs->pt.y << "KEY:" << pmshs->dwExtraInfo;
    return CallNextHookEx(MouseHook::instance()->GetMouseHook(), nCode, wParam, lParam);
}

MouseHook::MouseHook()
    :m_bHookEnable(false)
{
}

HHOOK MouseHook::GetMouseHook()
{
    return m_mouseHook;
}

HHOOK MouseHook::GetKeyHook()
{
    return m_keyHook;
}

void MouseHook::setMouseClick(long x, long y)
{
    emit mouseClicked(x, y);
}
void MouseHook::setKeyStart()
{
    emit keyStart();
}

void MouseHook::setKeyPause()
{
    emit keyPause();
}

void MouseHook::clickKey(long x, long y)
{
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,x, y, 0, 0);
    //mouse_event(WM_LBUTTONDOWN, -60, 0, 0, 0);
    //mouse_event(WM_LBUTTONUP, 0, 0, 0, 0);
    qDebug() << "click  x: " << x << " y: " << y << endl;
}

//ж�ع���
void  MouseHook::unHook()
{
    if (!m_bHookEnable)
        return;

    //UnhookWindowsHookEx(m_keyHook);
    UnhookWindowsHookEx(m_mouseHook);
    m_bHookEnable = false;
}
//��װ����,���øú�������װ����
void MouseHook::setHook()
{
    if (m_bHookEnable)
        return;
    //�������ײ㹳��,��ҪDLL�Ϳ���ȫ��
    //�ײ���̹���
    //m_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
    //�ײ���깳��
    m_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);
    m_bHookEnable = true;
}

void MouseHook::setHookKey()
{
    m_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
}
void MouseHook::unHookKey()
{
    UnhookWindowsHookEx(m_mouseHook);
}

void MouseHook::pressKeyPaste()
{
    //setHookKey();
    //keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('v', 0, 0, 0);

    //keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
    //unHookKey();

    qDebug() << "press key" << endl;
}


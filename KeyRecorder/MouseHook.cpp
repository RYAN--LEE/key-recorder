#include "MouseHook.h"
#include<QDebug>


//键盘钩子过程
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
    KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
    qDebug() << "key:" << pkbhs->vkCode;
    if (pkbhs->vkCode == VK_F12 || pkbhs->vkCode == VK_F10)
    {
     /*   void unHook();
        qApp->quit();*/
    }
    return 0;//返回1表示截取消息不再传递,返回0表示不作处理,消息继续传递

}
//鼠标钩子过程
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

void MouseHook::clickKey(long x, long y)
{
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,x, y, 0, 0);
    //mouse_event(WM_LBUTTONDOWN, -60, 0, 0, 0);
    //mouse_event(WM_LBUTTONUP, 0, 0, 0, 0);
}

//卸载钩子
void  MouseHook::unHook()
{
    if (!m_bHookEnable)
        return;

    //UnhookWindowsHookEx(m_keyHook);
    UnhookWindowsHookEx(m_mouseHook);
    m_bHookEnable = false;
}
//安装钩子,调用该函数即安装钩子
void MouseHook::setHook()
{
    if (m_bHookEnable)
        return;
    //这两个底层钩子,不要DLL就可以全局
    //底层键盘钩子
    //m_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
    //底层鼠标钩子
    m_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);
    m_bHookEnable = true;
}


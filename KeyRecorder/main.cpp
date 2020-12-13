#include <QtWidgets/QApplication>
#include "KeyRecorder.h"
#include <atlstr.h>

void checkoutInstance()
{
    CString strMutexName = "KeyRecorderInstance";
    HANDLE hMutex = CreateMutex(NULL, FALSE, strMutexName);
    if (hMutex)
    {
        if (ERROR_ALREADY_EXISTS == ::GetLastError())
        {//���������ǰ��ʵ��,���˳�

            HWND hWnd = ::FindWindow(NULL, L"�����ط�");
            if (hWnd)
            {
                ::ShowWindow(hWnd, SW_SHOWNORMAL);
                ::SetForegroundWindow(hWnd);
                ::BringWindowToTop(hWnd);
                ::FlashWindow(hWnd, TRUE);
            }
            exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
    checkoutInstance();

    QApplication a(argc, argv);
    KeyRecorder w;
    w.show();
    return a.exec();
}

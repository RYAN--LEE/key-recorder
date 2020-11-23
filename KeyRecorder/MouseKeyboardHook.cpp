#include "MouseKeyboardHook.h"

HMODULE hDll = ::GetModuleHandle(LPCTSTR("HOOK.dll"));
typedef void(*f1) (WPARAM wParam, LPARAM lParam);
void(*pFunc) (WPARAM wParam, LPARAM lParam);

//共享内存变量
#pragma data_seg("MouseKeyboardHook")  
HHOOK g_hMouseHook = NULL;
HHOOK g_hKeyboardHook = NULL;
HWND g_lhDisplayWnd = NULL;
#pragma data_seg()  

int UnSetHook(void);
int SetHook(HWND main);

//钩子函数的格式 LRESULT CALLBACK 函数名(int 钩子类型, WPARAM wParam, LPARAM lParam);
//处理鼠标的钩子函数  
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
//处理键盘的钩子函数
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

//这个是获取DLL的内存地址，可以重复使用，当做模版  
HMODULE WINAPI ModuleFromAddress(PVOID pv);

HWND m_hCursorWnd;
void CreateCursonIcon();
long __stdcall WindowProcedure(HWND hwnd, unsigned int msg, WPARAM wp, LPARAM lp);

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_MOUSEMOVE) //只处理WM_MOUSEMOVE消息
	{

		MOUSEHOOKSTRUCT* mhookstruct; //鼠标HOOK结构体

		mhookstruct = (MOUSEHOOKSTRUCT*)lParam;

		POINT pt = mhookstruct->pt;

		//将当前鼠标坐标点的x，y坐标作为参数向主程序窗口发送消息

		PostMessage(g_lhDisplayWnd, WM_MOUSEMOVE, MK_CONTROL, MAKELPARAM(pt.x, pt.y));
		//SetWindowPos(m_hCursorWnd, 0, pt.x - 20, pt.y - 20, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	pFunc(wParam, lParam);
	//PostMessage(g_lhDisplayWnd, WH_MOUSE_LL, wParam, lParam);

	return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	pFunc(wParam, lParam);
	//PostMessage(g_lhDisplayWnd, WM_KEYDOWN, wParam, lParam);
	return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}

//安装钩子函数  
MOUSEKEYBOARDHOOK_API int SetHook(HWND main)
{
	g_lhDisplayWnd = main;
	g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hDll, 0);
	g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, hDll, 0);

	//CreateCursonIcon();
	return g_hMouseHook && g_hKeyboardHook ? 1 : 0;
}

MOUSEKEYBOARDHOOK_API int UnSetHook(void)
{
	g_lhDisplayWnd = NULL;
	BOOL b1 = UnhookWindowsHookEx(g_hMouseHook);
	BOOL b2 = UnhookWindowsHookEx(g_hKeyboardHook);
	//::DestroyWindow(m_hCursorWnd);
	return b1 && b2 ? 1 : 0;
}

HMODULE WINAPI ModuleFromAddress(PVOID pv)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(pv, &mbi, sizeof(mbi)) != 0) {
		return (HMODULE)mbi.AllocationBase;
	}
	else {
		return NULL;
	}
}

void pro(void(*f1) (WPARAM wParam, LPARAM lParam))
{
	pFunc = f1;
}

void CreateCursonIcon()
{

	WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, (WNDPROC)WindowProcedure,
		0, 0, GetModuleHandle(0), LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW), CreateSolidBrush(RGB(0, 255, 0)),
		0, L"cursor", LoadIcon(0, IDI_APPLICATION) };

	if (!::GetClassInfoEx(GetModuleHandle(0), L"cursor", &wndclass))
		RegisterClassEx(&wndclass);

	POINT p;
	GetCursorPos(&p);
	m_hCursorWnd = CreateWindowEx(WS_EX_TOOLWINDOW, L"cursor", L" 是", WS_POPUP | WS_VISIBLE,
		p.x - 20, p.y - 20, 40, 40,
		NULL, NULL, GetModuleHandle(0), NULL);

	//starthook(m_windowHandl);

	RECT rect;
	GetClientRect(m_hCursorWnd, &rect);
	HRGN hRgn = CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
	SetWindowRgn(m_hCursorWnd, hRgn, TRUE);


	SetWindowLong(m_hCursorWnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE);
	SetLayeredWindowAttributes(m_hCursorWnd, RGB(255, 255, 255), 155, LWA_ALPHA);

	SetWindowPos(m_hCursorWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

long __stdcall WindowProcedure(HWND hwnd, unsigned int msg, WPARAM wp, LPARAM lp)
{

	return DefWindowProc(hwnd, msg, wp, lp);
}
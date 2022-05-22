#include "Input.h"
#include<QTextCodec>

void Input::InputString(QString utf8str)
{
	QTextCodec* pTextCodeUTF = QTextCodec::codecForName("UTF-8");
	QString byteStr = pTextCodeUTF->toUnicode(utf8str.toUtf8());
	//QString data = QString::fromLocal8Bit(str.toStdString().c_str());
	wchar_t wstr[256] = { 0 };
	int nLen = byteStr.toWCharArray(wstr);
	InputData(wstr, nLen);
}
void Input::InputData(const wchar_t* data, int len)
{
	short vk;
	bool shift;

	//wchar_t data[512] = { 0 };
	//int len = str.toWCharArray(data);
	for (int i = 0; i < len; i++)
	{
		if (data[i] >= 0 && data[i] < 256) //ascii×Ö·û
		{
			vk = VkKeyScanW(data[i]);
			if (vk == -1)
			{
				InputUnicode(data[i]);
			}
			else
			{
				if (vk < 0)
				{
					vk = ~vk + 0x1;
				}
				shift = vk >> 8 & 0x1;
				if (GetKeyState(VK_CAPITAL) & 0x1)
				{
					if (data[i] >= 'a' && data[i] <= 'z' || data[i] >= 'A' && data[i] <= 'Z')
					{
						shift = !shift;
					}
				}

				InputAscii(vk & 0xFF, shift);
			}
		}
		else //unicode×Ö·û
		{
			InputUnicode(data[i]);
		}
	}
}


void Input::InputAscii(wchar_t data, bool shift)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));

	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		SendInput(1, input, sizeof(INPUT));
	}

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = data;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(2, input, sizeof(INPUT));

	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		input[0].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, input, sizeof(INPUT));
	}
}


void Input::InputUnicode(wchar_t data)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0;
	input[0].ki.wScan = data;
	input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0;
	input[1].ki.wScan = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;

	SendInput(2, input, sizeof(INPUT));
}
#pragma once

#include <QString>
//#include <winable.h>
#include <atlconv.h>


class Input
{
public:
	void InputString(QString str);
	void InputData(const wchar_t* data, int len);
	void InputAscii(wchar_t data, bool shift);
	void InputUnicode(wchar_t data);
};


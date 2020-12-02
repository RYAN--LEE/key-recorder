#pragma once

#include <QObject>
#include <Windows.h>


class MouseHook : public QObject
{
	Q_OBJECT

public:
	static MouseHook* instance()
	{
		static MouseHook instance;
		return &instance;
	}

	HHOOK GetMouseHook();
	HHOOK GetKeyHook();

	void setMouseClick(long x, long y);
	void setKeyPause();
	void setHook();
	void unHook();

	void setHookKey();
	void unHookKey();


	void clickKey(long x, long y);

	void pressKeyPaste();

signals:
	void mouseClicked(long x, long y);
	void keyPause();

private:
	MouseHook();

private:
	HHOOK m_keyHook;
	HHOOK m_mouseHook;
	bool m_bHookEnable;
};


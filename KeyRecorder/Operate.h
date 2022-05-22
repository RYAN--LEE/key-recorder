#pragma once

#include "MouseHook.h"
#include "Input.h"
#include "KeyInfo.h"

class KeyInfo;
class IOperate
{
public:
	virtual void operate(KeyInfo* key) = 0;
};

class OperateNone : public IOperate
{
	void operate(KeyInfo* key) {}

};

class OperateClick : public IOperate
{
private:
	MouseHook* m_pMouseHook;
	int m_x;
	int m_y;
public:
	OperateClick(MouseHook* pMouseHook,int x, int y) 
		:m_pMouseHook(pMouseHook), m_x(x), m_y(y) {}
		
	void operate(KeyInfo* key)
	{
		m_pMouseHook->clickKey(m_x, m_y);
	}

};

class OperateInput : public IOperate
{
private:
	QString m_content;
	Input* m_pInput;

public:
	void operate(KeyInfo* key)
	{
		m_pInput->InputString(m_content);
	}
};

class OperatePopupClick : public IOperate
{
private:
	MouseHook* m_pMouseHook;
	int m_nIndex;
	int m_nItemHeight;
public:
	void operate(KeyInfo* key)
	{
		m_pMouseHook->clickKey(key->x(), key->y() + (m_nIndex + 1) * m_nItemHeight);
	}
};

class OperatePopupMove : public IOperate
{
private:
	MouseHook* m_pMouseHook;
	int m_nIndex;
	int m_nItemHeight;
public:
	void operate(KeyInfo* key)
	{
		m_pMouseHook->clickKey(key->x(), key->y() + m_nItemHeight);
		m_pMouseHook->clickKey(key->x(), key->y());
		for (int i = 0; i < m_nIndex; i++)
		{
			m_pMouseHook->pressKeyDonw();
		}
	}
};
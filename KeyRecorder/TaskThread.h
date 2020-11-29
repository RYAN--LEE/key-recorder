#pragma once

#include <QThread>
#include "KeyInfo.h"
#include "MouseHook.h"
#include <QMutex>

class TaskThread : public QThread
{
	Q_OBJECT

public:
	TaskThread(QObject* parent, MouseHook* pMouseHook);
	~TaskThread();

	void run();

	void play();
	void pause();
	void setKeyInfos(QVector<KeyInfo> vecKeyInfo);

private:
	bool handleCondition(QString& condition);

private:
	bool m_bPlay;
	QVector<KeyInfo>			m_vecKeyInfo;
	QMutex m_mutex;
	MouseHook* m_pMouseHook;

};

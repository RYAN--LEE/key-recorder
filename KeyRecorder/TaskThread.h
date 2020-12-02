#pragma once

#include <QThread>
#include "KeyInfo.h"
#include "MouseHook.h"
#include "Recongnizer.h"
#include "ScreenGraber.h"
#include <QMutex>

class TaskThread : public QThread
{
	Q_OBJECT

public:
	TaskThread(QObject* parent, MouseHook* pMouseHook, Recongnizer* pRecongnizer, ScreenGraber *pScreenGraber);
	~TaskThread();

	void run();

	void play();
	void pause();
	void setKeyInfos(QVector<KeyInfo> vecKeyInfo);

signals:
	void recongnizeValue(QString value);
	void imageMatched(QString image, bool status);

private:
	bool handleCondition(QString& condition);
	void inputData(QString& data);
	QString getTemplate(QString& status);
	bool checkScreenStatus(QString& status);

	QString getRoomNum(QString strName, QString strID);

private:
	bool m_bPlay;
	QVector<KeyInfo>			m_vecKeyInfo;
	QMutex m_mutex;
	MouseHook* m_pMouseHook;
	Recongnizer* m_pRecongnizer;
	ScreenGraber* m_pScreenGraber;

	QString m_strName;
	QString m_strID;
};

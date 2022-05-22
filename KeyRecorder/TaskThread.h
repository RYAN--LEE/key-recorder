#pragma once

#include <QThread>
#include "KeyInfo.h"
#include "MouseHook.h"
#include "Input.h"
#include "Recongnizer.h"
#include "ScreenGraber.h"
#include "StepCfg.h"
#include <QMutex>

class TaskContentKey
{
public:
	QVector<KeyInfo>		m_vecKeyInfo;
	QMap<QString, Content>  m_contents;
};
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
	bool checkScreenStatus(const QString& status, QRect& matchRect);
	QString recongnizeText(QString & imgDir);
	void inputData(QString& data);

	void clearTask();
	void addTask(TaskContentKey taskInfo);
	void executTask(TaskContentKey& task);

signals:
	void recongnizeValue(QString value);
	void imageMatched(QString image, bool status);
	void roomInputed(QString roomNum);
	void createCardFinish(bool isSuccess);
	void stepStatusChange(int step, bool success, QString result);
	void showMsg(QString title, QString info);

private:
	void maxmizeWindow();
	bool beforClick(const QString& condition, QRect& ajustRect);
	bool handleCondition(KeyInfo& curStep, QMap<QString, Content>& curContent, QRect& ajustRect);
	bool handleInput(KeyInfo& curStep, QMap<QString, Content>& curContent, QRect& ajustRect);
	bool handlePopup(KeyInfo& curStep, QMap<QString, Content>& curContent, QRect& ajustRect);
	QString getTemplate(const QString& status);

	QString getRoomNum(QString strName, QString strID);
	bool makeCard(QString strName, QString strID, QString roomNum);
	void adjustPoint(KeyInfo &keyInfo, QRect adjustRect, QRect matchRect);

private:
	bool m_bPlay;
	QVector<KeyInfo>			m_vecKeyInfo;
	QMutex m_mutex;
	MouseHook* m_pMouseHook;
	Recongnizer* m_pRecongnizer;
	ScreenGraber* m_pScreenGraber;

	QString m_strName;
	QString m_strID;
	QString m_strRoomNum;

	Input* m_pInput;
	//QMap<QString, Content> m_mapCurentContents;
	//QList<QMap<QString, Content> > m_tasks;

	QList<TaskContentKey>  m_tasks;
};

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyRecorder.h"
#include <QEvent>
#include <QVector>
#include <QPoint>
#include <QTime>
#include "MouseHook.h"
#include "FormMoreInfo.h"
#include "ScreenGraber.h"
#include "Recongnizer.h"
#include "KeyStore.h"
#include "TaskThread.h"


class KeyRecorder : public QMainWindow
{
	Q_OBJECT

public:
	KeyRecorder(QWidget* parent = Q_NULLPTR);
	~KeyRecorder() {};

signals:
	void startPlay(QVector<KeyInfo> points);

public slots:
	void recieveClicked(long x, long y);
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonPlay_clicked();
	void on_pushButtonPause_clicked();
	void on_pushButtonMore_clicked();
	void playClick(QVector<KeyInfo> points);

	void on_pushButtonStartRcd_clicked();
	void on_pushButtonStopRcd_clicked();
	void on_pushButtonTest_clicked();

private:
	bool checkScreenStatus(int status);
	QString getTemplate(int status);

private:
	Ui::KeyRecorderClass	ui;
	MouseHook* m_pMouseHook;
	TaskThread* m_pTaskThread;
	QVector<KeyInfo>			m_vecPoint;
	QTime  m_LastClickTime;

	KeyStore* m_pKeyStore;
	ScreenGraber* m_pScreenGraber;
	Recongnizer* m_pRecongnizer;

	//bool m_bPlay;

	bool m_bRecord;
	QPoint m_recordBegin;
	QPoint m_recordEnd;
	QPoint m_recordPos;

	long					m_nPlayInterval;
	FormMoreInfo* m_pFormMoreInfo;
};

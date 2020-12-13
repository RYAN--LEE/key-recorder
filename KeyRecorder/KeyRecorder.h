#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyRecorder.h"
#include <QEvent>
#include <QVector>
#include <QPoint>
#include <QTime>
#include "MouseHook.h"
#include "ScreenGraber.h"
#include "Recongnizer.h"
#include "KeyStore.h"
#include "TaskThread.h"
#include "FormOperate.h"
#include "FormConfig.h"


class KeyRecorder : public QMainWindow
{
	Q_OBJECT

public:
	KeyRecorder(QWidget* parent = Q_NULLPTR);
	~KeyRecorder();

signals:
	void startPlay(QVector<KeyInfo> points);

public slots:
	void recieveRecongnizeValue(QString value);
	void recieveMatchImage(QString image, bool status);
	void recieveRoomNum(QString roomNum);
	void recieveStatus(int step, bool success, QString result);

	void on_pushButtonPlay_clicked();
	void on_pushButtonPause_clicked();
	void on_pushButtonConfig_clicked();

private:
	void initTray();

private:
	Ui::KeyRecorderClass	ui;
	bool m_bStar;
	MouseHook* m_pMouseHook;
	TaskThread* m_pTaskThread;
	QVector<KeyInfo>			m_vecPoint;
	QTime  m_LastClickTime;

	KeyStore* m_pKeyStore;
	ScreenGraber* m_pScreenGraber;
	Recongnizer* m_pRecongnizer;
};

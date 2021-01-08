#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyRecorder.h"
#include <QEvent>
#include <QVector>
#include <QPoint>
#include <QTime>
#include <QMouseEvent>
#include <QSystemTrayIcon>
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
	void recieveCreateCardFinish(bool isSuccess);
	void recieveStatus(int step, bool success, QString result);
	void showMsg(QString title, QString info);

	void onActivated(QSystemTrayIcon::ActivationReason reason);

	void on_pushButtonPlay_clicked();
	void on_pushButtonPause_clicked();
	void on_pushButtonConfig_clicked();
	void on_pushButtonClose_clicked();


protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
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

	bool m_bLeftBtnClk;
	QPoint m_Press;
	QPoint m_Move;
};

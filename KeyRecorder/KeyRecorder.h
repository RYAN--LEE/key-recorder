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
#include "FormOperate.h"


class KeyRecorder : public QMainWindow
{
	Q_OBJECT

public:
	KeyRecorder(QWidget* parent = Q_NULLPTR);
	~KeyRecorder();

signals:
	void startPlay(QVector<KeyInfo> points);

public slots:
	void recieveClicked(long x, long y);
	void changeItem(QTreeWidgetItem* item, int column);
	void recieveRecongnizeValue(QString value);
	void recieveMatchImage(QString image, bool status);

	void on_pushButtonPlay_clicked();
	void on_pushButtonPause_clicked();

	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	//void playClick(QVector<KeyInfo> points);
	
	void on_pushButtonRecoImg_clicked();
	void on_pushButtonRecoText_clicked();
	void on_pushButtonRom_clicked();
	void on_pushButtonPMS_clicked();

	void on_pushButtonTest_clicked();

	void showOperateForm();
	void imageMatchOperateFinish(QString name);
	void textMatchOperateFinish(QRect rect);
	void roomOperateFinish(QString room);

private:
	void init();
	void initTray();
	void setTreeWidget(QVector<KeyInfo> & vecPoint);

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
	FormOperate* m_pFormOperate;
};

#pragma once

#include <QWidget>
#include "ui_FormConfig.h"
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

class FormConfig : public QWidget
{
	Q_OBJECT

public:
	FormConfig(QWidget *parent = Q_NULLPTR);
	~FormConfig();

signals:
	void startPlay(QVector<KeyInfo> points);

public slots:
	void recieveClicked(long x, long y);
	void changeItem(QTreeWidgetItem* item, int column);

	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();

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
	void setTreeWidget(QVector<KeyInfo>& vecPoint);

private:
	Ui::FormConfig ui;
	MouseHook* m_pMouseHook;
	QVector<KeyInfo> m_vecPoint;
	QTime  m_LastClickTime;

	KeyStore* m_pKeyStore;
	ScreenGraber* m_pScreenGraber;
	Recongnizer* m_pRecongnizer;

	bool m_bRecord;
	QPoint m_recordBegin;
	QPoint m_recordEnd;
	QPoint m_recordPos;

	FormOperate* m_pFormOperate;
};

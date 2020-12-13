#pragma once

#include <QWidget>
#include "ui_FormConfig.h"
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

class FormConfig : public QWidget
{
	Q_OBJECT

public:
	FormConfig(TaskThread *pTaskThread, QWidget *parent = Q_NULLPTR);
	~FormConfig();

signals:
	void startPlay(QVector<KeyInfo> points);

public slots:
	void recieveClicked(long x, long y);
	void changeItem(QTreeWidgetItem* item, int column);
	
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonCancel_clicked();

	void on_pushButtonRecoImg_clicked();
	void on_pushButtonRecoText_clicked();

	void on_lineEditPMS_editingFinished();
	void on_lineEditPMSType_editingFinished();
	void on_lineEditGroupCode_editingFinished();
	void on_lineEditHotelCode_editingFinished();
	void on_pushButtonPMS_clicked();

	void showOperateForm();
	void showBeforeOperateForm();
	void imageMatchOperateFinish(int formType, QString name, QRect rect);
	void textMatchOperateFinish(int formType, QRect rect);
	void roomOperateFinish(int formType, QString room);

private:
	void init();
	void setTreeWidget(QVector<KeyInfo>& vecPoint);
	QString getMac();

private:
	Ui::FormConfig ui;
	MouseHook* m_pMouseHook;
	QVector<KeyInfo> m_vecPoint;
	QTime  m_LastClickTime;

	KeyStore* m_pKeyStore;
	ScreenGraber* m_pScreenGraber;
	Recongnizer* m_pRecongnizer;

	bool m_bRecord;

	FormOperate* m_pFormOperate;
	TaskThread* m_pTaskThread;
};

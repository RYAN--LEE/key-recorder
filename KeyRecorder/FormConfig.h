#pragma once

#include <QWidget>
#include "ui_FormConfig.h"
#include <QEvent>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
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
	void showCfg();
	void closePwd();

	void on_pushButtonAbort_clicked();
	void on_pushButtonClose_clicked();

	void on_checkBoxMore_stateChanged(int state);
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonCancel_clicked();
	void on_pushButtonLoad_clicked();

	void on_pushButtonRecoImg_clicked();
	void on_pushButtonRecoText_clicked();
	void on_pushButtonInput_clicked();

	void on_lineEditPMS_editingFinished();
	void on_lineEditPMSType_editingFinished();
	void on_lineEditGroupCode_editingFinished();
	void on_lineEditHotelCode_editingFinished();
	void on_pushButtonPMS_clicked();
	void on_pushButtonCreateCard_clicked();


	void selectNameFinish(QString name);
	void showOperateForm();
	void showBeforeOperateForm();
	void imageMatchOperateFinish(int formType, QString name, QRect rect);
	void textMatchOperateFinish(int formType, QRect rect);
	void roomOperateFinish(int formType, QString room);
	void createCardOperateFinish(int formType, QString room);

	void testInput();

protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
private:
	void init();
	void setTreeWidget(QVector<KeyInfo>& vecPoint);
	QString getMac();
	void stopReord();

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

	bool m_bLeftBtnClk;
	QPoint m_Press;
	QPoint m_Move;
};

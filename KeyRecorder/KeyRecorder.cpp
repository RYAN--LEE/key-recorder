#include "KeyRecorder.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QByteArray>
#include <QTreeWidgetItem>
#include <qtextcodec.h> 
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDesktopWidget>
#include <string.h>

#include "constant.h"
#include "ImageMacher.h"
#include "KeyInfo.h"
#include "Utils.h"
#include "FormInform.h"

KeyRecorder::KeyRecorder(QWidget* parent)
	: QMainWindow(parent , Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
	, m_bStar(false)
	, m_bLeftBtnClk(false)
{
	ui.setupUi(this);
	initTray();
	setWindowIcon(QIcon(":/icon/logo.png"));
	ui.labelTip->setText(QString::fromLocal8Bit("按F9开始"));


	QDesktopWidget* desktop = QApplication::desktop();
	setGeometry(desktop->width() - 275, desktop->height() - 210, 270, 166);

	m_pMouseHook = MouseHook::instance();
	m_pMouseHook->setHookKey();

	m_pKeyStore = new KeyStore(this);


	m_pScreenGraber = new ScreenGraber(this);
	m_pRecongnizer = new Recongnizer();

	m_pTaskThread = new TaskThread(NULL, m_pMouseHook, m_pRecongnizer, m_pScreenGraber);
	m_pTaskThread->start();

	connect(m_pMouseHook, &MouseHook::keyPause, this, &KeyRecorder::on_pushButtonPause_clicked);
	connect(m_pMouseHook, &MouseHook::keyStart, this, &KeyRecorder::on_pushButtonPlay_clicked);
	connect(m_pTaskThread, &TaskThread::recongnizeValue, this, &KeyRecorder::recieveRecongnizeValue);
	connect(m_pTaskThread, &TaskThread::imageMatched, this, &KeyRecorder::recieveMatchImage);
	connect(m_pTaskThread, &TaskThread::roomInputed, this, &KeyRecorder::recieveRoomNum);
	connect(m_pTaskThread, &TaskThread::createCardFinish, this, &KeyRecorder::recieveCreateCardFinish);
	connect(m_pTaskThread, &TaskThread::stepStatusChange, this, &KeyRecorder::recieveStatus);
	connect(m_pTaskThread, &TaskThread::showMsg, this, &KeyRecorder::showMsg);
}

KeyRecorder::~KeyRecorder()
{
	m_pMouseHook->unHookKey();
}

void KeyRecorder::initTray()
{
	QSystemTrayIcon* pSystemTray = new QSystemTrayIcon(this);
	QMenu* menu = new QMenu(this);

	QAction* pShowAction = new QAction(QString::fromLocal8Bit("显示"), this);
	connect(pShowAction, SIGNAL(triggered(bool)), this, SLOT(showNormal()));
	menu->addAction(pShowAction);

	QAction* pConfigAction = new QAction(QString::fromLocal8Bit("设置"), this);
	connect(pConfigAction, SIGNAL(triggered(bool)), this, SLOT(on_pushButtonConfig_clicked()));
	menu->addAction(pConfigAction);

	QAction* pExitAction = new QAction(QString::fromLocal8Bit("退出"), this);
	connect(pExitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
	menu->addAction(pExitAction);
	// 设置系统托盘的上下文菜单
	pSystemTray->setContextMenu(menu);

	// 设置系统托盘提示信息、托盘图标
	pSystemTray->setToolTip(QString::fromLocal8Bit("旅业智能操作辅助系统"));
	pSystemTray->setIcon(QIcon(":/icon/logo.png"));

	// 连接信号槽
	connect(menu, SIGNAL(showWindow()), this, SLOT(showWindow()));
	connect(pSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));

	// 显示系统托盘
	pSystemTray->show();

	// 显示系统托盘提示信息
	pSystemTray->showMessage(QString::fromLocal8Bit("托盘标题"), QString::fromLocal8Bit("托盘显示内容"));
}

void KeyRecorder::onActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick)
	{
		showNormal();
	}
}

void KeyRecorder::recieveRecongnizeValue(QString value)
{
	ui.label->setText(value);
}

void KeyRecorder::recieveStatus(int step, bool success, QString result)
{
	QString strSuccess = QString::fromLocal8Bit("成功");
	QString strFailed = QString::fromLocal8Bit("失败");

	QString strState = QString::fromLocal8Bit("第%1步 %2 ").arg(step).arg(success? strSuccess : strFailed) + result;
	ui.label->setText(strState);
}


void KeyRecorder::recieveMatchImage(QString image, bool status)
{
	QString strState = status ? " true" : " false";
	//ui.label->setText(image + strState);
}

void KeyRecorder::recieveRoomNum(QString roomNum)
{
	if (roomNum == "failed")
	{
		on_pushButtonPause_clicked();
	}
	ui.label->setText("room:" + roomNum);
	//ui.labelPlayState->setText(roomNum);//todo
}

void KeyRecorder::recieveCreateCardFinish(bool isSuccess)
{
	QString text = isSuccess ? QString::fromLocal8Bit("制卡成功") : QString::fromLocal8Bit("制卡失败");
	ui.label->setText(text);

	if (!isSuccess)
	{
		on_pushButtonPause_clicked();
	}
}

void KeyRecorder::on_pushButtonPlay_clicked()
{
	if (m_bStar)
	{
		return;
	}
	m_bStar = true;
	QVector<KeyInfo> vecPoint = m_pKeyStore->getKeys();
	m_pTaskThread->setKeyInfos(vecPoint);
	m_pTaskThread->play();

	ui.labelPlayState->setText(QString::fromLocal8Bit("回放"));
	ui.labelTip->setText(QString::fromLocal8Bit("按F10停止"));
}

void KeyRecorder::on_pushButtonPause_clicked()
{
	if (!m_bStar)
	{
		return;
	}
	m_bStar = false;
	m_pTaskThread->pause();
	ui.labelPlayState->setText(QString::fromLocal8Bit("停止"));
	ui.labelTip->setText(QString::fromLocal8Bit("按F9开始"));
}

void KeyRecorder::on_pushButtonConfig_clicked()
{
	FormConfig* pFormConfig = new FormConfig(m_pTaskThread, this);
	pFormConfig->show();
}

void KeyRecorder::on_pushButtonClose_clicked()
{
	this->hide();
}

void KeyRecorder::showMsg(QString title, QString info)
{
	FormInform::showInfo(title, info, this);
}


void KeyRecorder::mousePressEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton &&
		ui.widgetTitle->frameGeometry().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		m_bLeftBtnClk = true;
	}
	event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void KeyRecorder::mouseReleaseEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton) {
		m_bLeftBtnClk = false;
	}
	event->ignore();
}

void KeyRecorder::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bLeftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}
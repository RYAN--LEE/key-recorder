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
#include "Configure.h"


KeyRecorder::KeyRecorder(QWidget* parent)
	: QMainWindow(parent , Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
	, m_bStar(false)
{
	ui.setupUi(this);
	initTray();
	setWindowIcon(QIcon(":/icon/logo.png"));


	QDesktopWidget* desktop = QApplication::desktop();
	setGeometry(desktop->width() - 265, desktop->height() - 115, 270, 72);

	m_pMouseHook = MouseHook::instance();
	m_pMouseHook->setHookKey();

	m_pKeyStore = new KeyStore(this);


	m_pScreenGraber = new ScreenGraber(this);
	m_pRecongnizer = new Recongnizer();

	m_pTaskThread = new TaskThread(NULL, m_pMouseHook, m_pRecongnizer, m_pScreenGraber);
	m_pTaskThread->start();

	connect(m_pMouseHook, &MouseHook::keyPause, this, &KeyRecorder::on_pushButtonPause_clicked);
	connect(m_pTaskThread, &TaskThread::recongnizeValue, this, &KeyRecorder::recieveRecongnizeValue);
	connect(m_pTaskThread, &TaskThread::imageMatched, this, &KeyRecorder::recieveMatchImage);
	connect(m_pTaskThread, &TaskThread::roomInputed, this, &KeyRecorder::recieveRoomNum);

	maxmizeWindow();

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
	pSystemTray->setToolTip(QString::fromLocal8Bit("按键识别录制"));
	pSystemTray->setIcon(QIcon(":/icon/logo.png"));

	// 连接信号槽
	connect(menu, SIGNAL(showWindow()), this, SLOT(showWindow()));
	connect(pSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));

	// 显示系统托盘
	pSystemTray->show();

	// 显示系统托盘提示信息
	pSystemTray->showMessage(QString::fromLocal8Bit("托盘标题"), QString::fromLocal8Bit("托盘显示内容"));
}

HWND KeyRecorder::findWindow(QString name)
{
	//得到桌面窗口  
   HWND hd=GetDesktopWindow();  
 
   //得到屏幕上第一个子窗口  
   hd=GetWindow(hd,GW_CHILD);  
   char s[200]={0};  
 
   //循环得到所有的子窗口  
   while(hd!=NULL)  
   {  
       memset(s,0,200);  
       GetWindowText(hd, (LPTSTR)s,200);
       if (name == QString::fromStdWString((LPTSTR)s))
       {
		   //return hd;
       }
       qDebug() << "window: " << QString::fromStdWString((LPTSTR)s) << endl;
         
       hd=GetNextWindow(hd,GW_HWNDNEXT);  
   }  
 
   return NULL;
}
void KeyRecorder::maxmizeWindow()
{
	QString name = Configure::instance()->getWindowName();
	if (name.isEmpty())
	{
		return;
	}
	HWND hq = FindWindow(NULL, name.toStdWString().c_str());

	//if (hq == NULL) {
	//	hq = findWindow(name);
	//}

	if (hq != NULL)
	{
		ShowWindow(hq, SW_MAXIMIZE);
	}
}


void KeyRecorder::recieveRecongnizeValue(QString value)
{
	ui.label->setText(value);
}

void KeyRecorder::recieveMatchImage(QString image, bool status)
{
	QString strState = status ? " true" : " false";
	ui.label->setText(image + strState);
}

void KeyRecorder::recieveRoomNum(QString roomNum)
{
	ui.label->setText("room:" + roomNum);
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

	ui.labelPlayState->setText(QString::fromLocal8Bit("正在回放"));
}

void KeyRecorder::on_pushButtonPause_clicked()
{
	if (!m_bStar)
	{
		return;
	}
	m_bStar = false;
	m_pTaskThread->pause();
	ui.labelPlayState->setText(QString::fromLocal8Bit("回放停止"));
}

void KeyRecorder::on_pushButtonConfig_clicked()
{
	FormConfig* pFormConfig = new FormConfig(m_pTaskThread, this);
	pFormConfig->show();
}

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

	QAction* pShowAction = new QAction(QString::fromLocal8Bit("��ʾ"), this);
	connect(pShowAction, SIGNAL(triggered(bool)), this, SLOT(showNormal()));
	menu->addAction(pShowAction);

	QAction* pConfigAction = new QAction(QString::fromLocal8Bit("����"), this);
	connect(pConfigAction, SIGNAL(triggered(bool)), this, SLOT(on_pushButtonConfig_clicked()));
	menu->addAction(pConfigAction);

	QAction* pExitAction = new QAction(QString::fromLocal8Bit("�˳�"), this);
	connect(pExitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
	menu->addAction(pExitAction);
	// ����ϵͳ���̵������Ĳ˵�
	pSystemTray->setContextMenu(menu);

	// ����ϵͳ������ʾ��Ϣ������ͼ��
	pSystemTray->setToolTip(QString::fromLocal8Bit("����ʶ��¼��"));
	pSystemTray->setIcon(QIcon(":/icon/logo.png"));

	// �����źŲ�
	connect(menu, SIGNAL(showWindow()), this, SLOT(showWindow()));
	connect(pSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));

	// ��ʾϵͳ����
	pSystemTray->show();

	// ��ʾϵͳ������ʾ��Ϣ
	pSystemTray->showMessage(QString::fromLocal8Bit("���̱���"), QString::fromLocal8Bit("������ʾ����"));
}

HWND KeyRecorder::findWindow(QString name)
{
	//�õ����洰��  
   HWND hd=GetDesktopWindow();  
 
   //�õ���Ļ�ϵ�һ���Ӵ���  
   hd=GetWindow(hd,GW_CHILD);  
   char s[200]={0};  
 
   //ѭ���õ����е��Ӵ���  
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

	ui.labelPlayState->setText(QString::fromLocal8Bit("���ڻط�"));
}

void KeyRecorder::on_pushButtonPause_clicked()
{
	if (!m_bStar)
	{
		return;
	}
	m_bStar = false;
	m_pTaskThread->pause();
	ui.labelPlayState->setText(QString::fromLocal8Bit("�ط�ֹͣ"));
}

void KeyRecorder::on_pushButtonConfig_clicked()
{
	FormConfig* pFormConfig = new FormConfig(m_pTaskThread, this);
	pFormConfig->show();
}

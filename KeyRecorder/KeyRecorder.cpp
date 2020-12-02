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

#include "constant.h"
#include "ImageMacher.h"
#include "KeyInfo.h"
#include "Utils.h"


KeyRecorder::KeyRecorder(QWidget* parent)
	: QMainWindow(parent , Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
	ui.setupUi(this);
	initTray();

	QDesktopWidget* desktop = QApplication::desktop();
	setGeometry(desktop->width() - 265, desktop->height() - 115, 270, 72);

	m_pMouseHook = MouseHook::instance();
	m_pMouseHook->setHookKey();

	m_pKeyStore = new KeyStore(this);

	//m_pFormOperate = new FormOperate(this);
	//m_pFormOperate->hide();

	m_pScreenGraber = new ScreenGraber(this);
	m_pRecongnizer = new Recongnizer();

	m_pTaskThread = new TaskThread(NULL, m_pMouseHook, m_pRecongnizer, m_pScreenGraber);
	m_pTaskThread->start();

	//connect(m_pMouseHook, &MouseHook::mouseClicked, this, &KeyRecorder::recieveClicked);
	connect(m_pMouseHook, &MouseHook::keyPause, this, &KeyRecorder::on_pushButtonPause_clicked);
	connect(m_pTaskThread, &TaskThread::recongnizeValue, this, &KeyRecorder::recieveRecongnizeValue);
	connect(m_pTaskThread, &TaskThread::imageMatched, this, &KeyRecorder::recieveMatchImage);

	//connect(m_pFormOperate, &FormOperate::operateImageMatch, this, &KeyRecorder::imageMatchOperateFinish);
	//connect(m_pFormOperate, &FormOperate::operateTextMatch, this, &KeyRecorder::textMatchOperateFinish);
	//connect(m_pFormOperate, &FormOperate::operateRoom, this, &KeyRecorder::roomOperateFinish);

	//init();
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

//void KeyRecorder::init()
//{
//	connect(ui.treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(changeItem(QTreeWidgetItem*, int)));
//	ui.treeWidget->setColumnWidth(0, 40);
//	ui.treeWidget->setColumnWidth(1, 50);
//	ui.treeWidget->setColumnWidth(2, 50);
//	ui.treeWidget->setColumnWidth(3, 60);
//	ui.treeWidget->setColumnWidth(4, 80);
//	ui.treeWidget->setColumnWidth(5, 80);
//	ui.treeWidget->setColumnWidth(6, 100);
//	ui.treeWidget->setColumnWidth(7, 30);
//	m_vecPoint = m_pKeyStore->getKeys();
//	setTreeWidget(m_vecPoint);
//}
//
//void KeyRecorder::changeItem(QTreeWidgetItem* item, int column)
//{
//	if (!isVisible())
//	{
//		return;
//	}
//	QVariant val = item->data(0, Qt::UserRole);
//	KeyInfo info = val.value<KeyInfo>();
//
//	if (column == 1)
//	{
//		info.m_x = item->text(1).toInt();
//	}
//	else if (column == 2)
//	{
//		info.m_y = item->text(2).toInt();
//	}
//	else if (column == 3)
//	{
//		info.m_interval = item->text(3).toInt();
//	}
//	else if (column == 4)
//	{
//		info.m_nextID = item->text(4).toInt();
//	}
//	else if (column == 5)
//	{
//		info.m_breakID = item->text(5).toInt();
//	}
//	else if (column == 6)
//	{
//		info.m_strCondition = item->text(6);
//	}
//	else {
//		return;
//	}
//
//	QVariant value = QVariant();
//	value.setValue<KeyInfo>(info);
//	item->setData(0, Qt::UserRole, value);
//
//	for (int i = 0; i < m_vecPoint.size(); ++i)
//	{
//		if (m_vecPoint[i].m_id == info.m_id)
//		{
//			m_vecPoint[i] = info;
//			break;
//		}
//	}
//	m_pKeyStore->saveKeys(m_vecPoint);
//}
//
//
//void KeyRecorder::recieveClicked(long x, long y)
//{
//	ui.label->setText(QString("x:") + QString::number(x) + QString(" y:") + QString::number(y));
//
//	if (m_bRecord)
//	{
//		m_recordBegin = m_recordEnd;
//		m_recordEnd = m_recordPos;
//		m_recordPos = QPoint(x, y);
//	}
//	else {
//		QTime current = QTime::currentTime();
//		int interval = 0;
//		if (!m_LastClickTime.isNull())
//		{
//			interval = m_LastClickTime.msecsTo(current);
//		}
//
//		m_vecPoint.push_back(KeyInfo(m_vecPoint.size(), x, y, interval, m_vecPoint.size()+1, 0, QString("")));
//		m_LastClickTime = current;
//	}
//}

void KeyRecorder::recieveRecongnizeValue(QString value)
{
	ui.label->setText(value);
	//todo 
}

void KeyRecorder::recieveMatchImage(QString image, bool status)
{
	QString strState = status ? " true" : " false";
	ui.label->setText(image + strState);
}

void KeyRecorder::on_pushButtonPlay_clicked()
{
	QVector<KeyInfo> vecPoint = m_pKeyStore->getKeys();
	m_pTaskThread->setKeyInfos(vecPoint);
	m_pTaskThread->play();

	ui.labelPlayState->setText(QString::fromLocal8Bit("正在回放"));
}

void KeyRecorder::on_pushButtonPause_clicked()
{
	m_pTaskThread->pause();
	ui.labelPlayState->setText(QString::fromLocal8Bit("回放停止"));
}

void KeyRecorder::on_pushButtonConfig_clicked()
{
	FormConfig* pFormConfig = new FormConfig(this);
	pFormConfig->show();
}

//void KeyRecorder::on_pushButtonStart_clicked()
//{
//	m_vecPoint.clear();
//	m_pMouseHook->setHook();
//}
//
//void KeyRecorder::on_pushButtonStop_clicked()
//{
//	m_pMouseHook->unHook();
//
//	if (m_vecPoint.size() > 0)
//	{
//		// 去掉最后一次点击结束
//		m_vecPoint.pop_back();
//	}
//
//	m_pKeyStore->saveKeys(m_vecPoint);
//	setTreeWidget(m_vecPoint);
//	m_LastClickTime = QTime();
//}
//
//void KeyRecorder::setTreeWidget(QVector<KeyInfo>& vecPoint)
//{
//	ui.treeWidget->clear();
//	for (QVector<KeyInfo>::iterator itr = vecPoint.begin(); itr != vecPoint.end(); ++itr)
//	{
//		QTreeWidgetItem* item = new QTreeWidgetItem(ui.treeWidget, itr->toStringList());
//		item->setFlags(item->flags() | Qt::ItemIsEditable);
//		QVariant value = QVariant();
//		value.setValue<KeyInfo>(*itr);
//		item->setData(0, Qt::UserRole, value);
//		ui.treeWidget->addTopLevelItem(item);
//
//		QPushButton* add = new QPushButton(QString("+"),ui.treeWidget);
//		connect(add, &QPushButton::clicked, this, &KeyRecorder::showOperateForm);
//		ui.treeWidget->setItemWidget(item, 7, add);
//	}
//}
//
//void KeyRecorder::showOperateForm()
//{
//	m_pFormOperate->show();
//}

//void KeyRecorder::imageMatchOperateFinish(QString name)
//{
//	ui.treeWidget->currentItem()->setText(6, name);
//}
//void KeyRecorder::textMatchOperateFinish(QRect rect)
//{
//	QString strRect = Utils::encodeTextMatch(rect);
//	ui.treeWidget->currentItem()->setText(6, strRect);
//}
//void KeyRecorder::roomOperateFinish(QString room)
//{
//	ui.treeWidget->currentItem()->setText(6, room);
//}
//
//void KeyRecorder::on_pushButtonRecoImg_clicked()
//{
//	
//}
//void KeyRecorder::on_pushButtonRecoText_clicked()
//{
//
//}
//void KeyRecorder::on_pushButtonRom_clicked()
//{
//
//}
//void KeyRecorder::on_pushButtonPMS_clicked()
//{
//
//}
//
//void KeyRecorder::on_pushButtonTest_clicked()
//{
//	QRect rect(m_recordBegin, m_recordEnd);
//	qDebug() << "record rect:" << rect;
//	QByteArray bytes;
//	m_pScreenGraber->grabScreen(rect.x(), rect.y(), rect.width(), rect.height(), bytes);
//
//	bool ok = false;
//	int nThreshold = ui.lineEditThreshold->text().toInt(&ok);
//	if (!ok) {
//		nThreshold = 100;
//	}
//
//	QByteArray grayBytes;
//	ImageMacher macher;
//	macher.grayImage(bytes, grayBytes, nThreshold);
//
//	QString strResult = m_pRecongnizer->DoRecongnize(grayBytes.data(), grayBytes.count());
//
//	ui.labelResult->setText(strResult);
//}

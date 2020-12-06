#include "FormConfig.h"
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
#include <QMessageBox>
#include <QDesktopWidget>
#include <QNetworkInterface>

#include "constant.h"
#include "ImageMacher.h"
#include "KeyInfo.h"
#include "Utils.h"
#include "Configure.h"
#include "HTTPClient.h"

FormConfig::FormConfig(TaskThread* pTaskThread, QWidget *parent)
	: QWidget(parent , Qt::Dialog)
	, m_bRecord(false)
	, m_pTaskThread(pTaskThread)
{
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);

	m_pMouseHook = MouseHook::instance();
	m_pMouseHook->setHookKey();

	m_pKeyStore = new KeyStore(this);

	m_pFormOperate = new FormOperate(this);
	m_pFormOperate->hide();

	m_pScreenGraber = new ScreenGraber(this);
	m_pRecongnizer = new Recongnizer();

	connect(m_pMouseHook, &MouseHook::mouseClicked, this, &FormConfig::recieveClicked);

	connect(m_pFormOperate, &FormOperate::operateImageMatch, this, &FormConfig::imageMatchOperateFinish);
	connect(m_pFormOperate, &FormOperate::operateTextMatch, this, &FormConfig::textMatchOperateFinish);
	connect(m_pFormOperate, &FormOperate::operateRoom, this, &FormConfig::roomOperateFinish);

	init();
}

FormConfig::~FormConfig()
{
}


void FormConfig::init()
{
	connect(ui.treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(changeItem(QTreeWidgetItem*, int)));
	ui.treeWidget->setColumnWidth(0, 40);
	ui.treeWidget->setColumnWidth(1, 50);
	ui.treeWidget->setColumnWidth(2, 50);
	ui.treeWidget->setColumnWidth(3, 60);
	ui.treeWidget->setColumnWidth(4, 80);
	ui.treeWidget->setColumnWidth(5, 80);
	ui.treeWidget->setColumnWidth(6, 100);
	ui.treeWidget->setColumnWidth(7, 30);
	m_vecPoint = m_pKeyStore->getKeys();
	setTreeWidget(m_vecPoint);

	Configure* cfg = Configure::instance();
	ui.lineEditPMS->setText(cfg->getUrl());
	ui.lineEditPMSType->setText(cfg->getPMSType());
	ui.lineEditGroupCode->setText(cfg->getGroupCode());
	ui.lineEditHotelCode->setText(cfg->getHotelCode());
	QString deviceID = cfg->getDeviceID();
	if (deviceID.isEmpty())
	{
		deviceID = getMac();
		cfg->setDeviceID(deviceID);
	}
	ui.labelDeviceID->setText(deviceID);

	QDesktopWidget* desktop = QApplication::desktop();
	setGeometry(desktop->width() - 550, desktop->height() - 600, 550, 460);
}

void FormConfig::changeItem(QTreeWidgetItem* item, int column)
{
	if (!isVisible())
	{
		return;
	}
	QVariant val = item->data(0, Qt::UserRole);
	KeyInfo info = val.value<KeyInfo>();

	if (column == 1)
	{
		info.m_x = item->text(1).toInt();
	}
	else if (column == 2)
	{
		info.m_y = item->text(2).toInt();
	}
	else if (column == 3)
	{
		info.m_interval = item->text(3).toInt();
	}
	else if (column == 4)
	{
		info.m_nextID = item->text(4).toInt();
	}
	else if (column == 5)
	{
		info.m_breakID = item->text(5).toInt();
	}
	else if (column == 6)
	{
		info.m_strCondition = item->text(6);
	}
	else {
		return;
	}

	QVariant value = QVariant();
	value.setValue<KeyInfo>(info);
	item->setData(0, Qt::UserRole, value);

	for (int i = 0; i < m_vecPoint.size(); ++i)
	{
		if (m_vecPoint[i].m_id == info.m_id)
		{
			m_vecPoint[i] = info;
			break;
		}
	}
	m_pKeyStore->saveKeys(m_vecPoint);
}


void FormConfig::recieveClicked(long x, long y)
{
	ui.label->setText(QString("x:") + QString::number(x) + QString(" y:") + QString::number(y));

	QTime current = QTime::currentTime();
	int interval = 0;
	if (!m_LastClickTime.isNull())
	{
		interval = m_LastClickTime.msecsTo(current);
	}

	m_vecPoint.push_back(KeyInfo(m_vecPoint.size(), x, y, interval, m_vecPoint.size() + 1, 0, QString("")));
	m_LastClickTime = current;
}

void FormConfig::on_pushButtonStart_clicked()
{
	if (m_bRecord)
	{
		return;
	}
	m_bRecord = true;
	m_vecPoint.clear();
	m_pMouseHook->setHook();
}

void FormConfig::on_pushButtonCancel_clicked()
{
	m_bRecord = false;
	m_vecPoint = m_pKeyStore->getKeys();
	m_pMouseHook->unHook();
}


void FormConfig::on_pushButtonStop_clicked()
{
	if (!m_bRecord)
	{
		return;
	}
	m_bRecord = false;
	m_pMouseHook->unHook();

	if (m_vecPoint.size() > 0)
	{
		// 去掉最后一次点击结束
		m_vecPoint.pop_back();
	}

	m_pKeyStore->saveKeys(m_vecPoint);

	setTreeWidget(m_vecPoint);

	//save(m_vecPoint, 0, m_vecPoint.size()-1);
	m_LastClickTime = QTime();
}

void FormConfig::setTreeWidget(QVector<KeyInfo>& vecPoint)
{
	ui.treeWidget->clear();
	for (QVector<KeyInfo>::iterator itr = vecPoint.begin(); itr != vecPoint.end(); ++itr)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(ui.treeWidget, itr->toStringList());
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		QVariant value = QVariant();
		value.setValue<KeyInfo>(*itr);
		item->setData(0, Qt::UserRole, value);
		ui.treeWidget->addTopLevelItem(item);

		QPushButton* add = new QPushButton(QString("+"), ui.treeWidget);
		connect(add, &QPushButton::clicked, this, &FormConfig::showOperateForm);
		ui.treeWidget->setItemWidget(item, 7, add);
	}
}

void FormConfig::showOperateForm()
{
	m_pFormOperate->show();
}

void FormConfig::imageMatchOperateFinish(QString name)
{
	ui.treeWidget->currentItem()->setText(6, name);
}
void FormConfig::textMatchOperateFinish(QRect rect)
{
	QString strRect = Utils::encodeTextMatch(rect);
	ui.treeWidget->currentItem()->setText(6, strRect);
}
void FormConfig::roomOperateFinish(QString room)
{
	ui.treeWidget->currentItem()->setText(6, room);
}

void FormConfig::on_pushButtonRecoImg_clicked()
{
	QString data = ui.lineEditRecoImg->text();
	QRect matchRect;
	bool bRet = m_pTaskThread->checkScreenStatus(data, matchRect);
	ui.labelRecoImgResult->setText(bRet?"true":"false");
}
void FormConfig::on_pushButtonRecoText_clicked()
{
	QString data = ui.lineEditRecoText->text();
	QString result = m_pTaskThread->recongnizeText(data);
	ui.labelResult->setText(result);
}

void FormConfig::on_lineEditPMS_editingFinished()
{
	QString url = ui.lineEditPMS->text();
	Configure::instance()->setUrl(url);
}

void FormConfig::on_lineEditPMSType_editingFinished()
{
	QString pmsType = ui.lineEditPMSType->text();
	Configure::instance()->setPMSType(pmsType);
}
void FormConfig::on_lineEditGroupCode_editingFinished()
{
	QString groupCode = ui.lineEditGroupCode->text();
	Configure::instance()->setGroupCode(groupCode);
}
void FormConfig::on_lineEditHotelCode_editingFinished()
{
	QString hotelCode = ui.lineEditHotelCode->text();
	Configure::instance()->setHotelCode(hotelCode);
}
void FormConfig::on_pushButtonPMS_clicked()
{
	HTTPClient client;
	QString name = ui.lineEditName->text();
	QString cardID = ui.lineEditCardID->text();
	QString roomNum;
	QString strRet = client.getRoomNumber(name, cardID, roomNum);
	if (roomNum.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("获取房间号"),
			QString::fromLocal8Bit("获取房间号失败:") + strRet);
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("获取房间号"),
			QString::fromLocal8Bit("获取房间号成功:") + roomNum);
	}
}

QString FormConfig::getMac()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
	int nCnt = nets.count();
	QString strMacAddr = "";
	for (int i = 0; i < nCnt; i++)
	{
		// 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			strMacAddr = nets[i].hardwareAddress();
			break;
		}
	}
	qDebug() << "Mac:" << strMacAddr;
	return strMacAddr;
}
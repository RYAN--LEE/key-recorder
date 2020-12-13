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


enum EColumnID
{
	ECOLUMN_ID,
	ECOLUMN_X,
	ECOLUMN_Y,
	ECOLUMN_INTERVAL,
	ECOLUMN_ADJUST,
	ECOLUMN_BEFORE_OPERATE,
	ECOLUMN_BEFORE_BUTTON,
	ECOLUMN_AFTER_OPERATE,
	ECOLUMN_AFTER_BUTTON,
	ECOLUMN_NEXT,
	ECOLUMN_BREAK_NEXT,
};

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
	ui.treeWidget->setColumnWidth(ECOLUMN_ID, 40);
	ui.treeWidget->setColumnWidth(ECOLUMN_X, 50);
	ui.treeWidget->setColumnWidth(ECOLUMN_Y, 50);
	ui.treeWidget->setColumnWidth(ECOLUMN_INTERVAL, 60);
	ui.treeWidget->setColumnWidth(ECOLUMN_NEXT, 80);
	ui.treeWidget->setColumnWidth(ECOLUMN_BREAK_NEXT, 80);
	ui.treeWidget->setColumnWidth(ECOLUMN_AFTER_OPERATE, 100);
	ui.treeWidget->setColumnWidth(ECOLUMN_AFTER_BUTTON, 30);
	ui.treeWidget->setColumnWidth(ECOLUMN_ADJUST, 80);
	ui.treeWidget->setColumnWidth(ECOLUMN_BEFORE_OPERATE, 80);
	ui.treeWidget->setColumnWidth(ECOLUMN_BEFORE_BUTTON, 30);
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
	QVariant val = item->data(ECOLUMN_ID, Qt::UserRole);
	KeyInfo info = val.value<KeyInfo>();

	if (column == ECOLUMN_X)
	{
		info.m_x = item->text(ECOLUMN_X).toInt();
	}
	else if (column == ECOLUMN_Y)
	{
		info.m_y = item->text(ECOLUMN_Y).toInt();
	}
	else if (column == ECOLUMN_INTERVAL)
	{
		info.m_interval = item->text(ECOLUMN_INTERVAL).toInt();
	}
	else if (column == ECOLUMN_NEXT)
	{
		info.m_nextID = item->text(ECOLUMN_NEXT).toInt();
	}
	else if (column == ECOLUMN_BREAK_NEXT)
	{
		info.m_breakID = item->text(ECOLUMN_BREAK_NEXT).toInt();
	}
	else if (column == ECOLUMN_AFTER_OPERATE)
	{
		info.m_strCondition = item->text(ECOLUMN_AFTER_OPERATE);
	}
	else if (column == ECOLUMN_ADJUST)
	{
		info.m_adjustRect = KeyInfo::string2Rect(item->text(ECOLUMN_ADJUST));
	}
	else if (column == ECOLUMN_BEFORE_OPERATE)
	{
		info.m_strBeforeCondition = item->text(ECOLUMN_BEFORE_OPERATE);
	}
	else {
		return;
	}

	QVariant value = QVariant();
	value.setValue<KeyInfo>(info);
	item->setData(ECOLUMN_ID, Qt::UserRole, value);

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
	int interval = 200;
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
		ui.treeWidget->setItemWidget(item, ECOLUMN_AFTER_BUTTON, add);

		QPushButton* addBefor = new QPushButton(QString("+"), ui.treeWidget);
		connect(addBefor, &QPushButton::clicked, this, &FormConfig::showBeforeOperateForm);
		ui.treeWidget->setItemWidget(item, ECOLUMN_BEFORE_BUTTON, addBefor);
	}
}

void FormConfig::showOperateForm()
{
	m_pFormOperate->setType(OPERATE_AFTER);
	m_pFormOperate->show();
}

void FormConfig::showBeforeOperateForm()
{
	m_pFormOperate->setType(OPERATE_BEFOR);
	m_pFormOperate->show();
}

void FormConfig::imageMatchOperateFinish(int formType, QString name, QRect rect)
{
	if (formType == OPERATE_BEFOR)
	{
		ui.treeWidget->currentItem()->setText(ECOLUMN_BEFORE_OPERATE, name);
		ui.treeWidget->currentItem()->setText(ECOLUMN_ADJUST, KeyInfo::rect2String(rect));
	} 
	else if (formType == OPERATE_AFTER)
	{
		ui.treeWidget->currentItem()->setText(ECOLUMN_AFTER_OPERATE, name);
		//ui.treeWidget->currentItem()->setText(ECOLUMN_ADJUST, KeyInfo::rect2String(rect));
	}
}
void FormConfig::textMatchOperateFinish(int formType, QRect rect)
{
	QString strRect = Utils::encodeTextMatch(rect);
	if (formType == OPERATE_BEFOR)
	{
		ui.treeWidget->currentItem()->setText(ECOLUMN_BEFORE_OPERATE, strRect);
	}
	else if (formType == OPERATE_AFTER)
	{
		ui.treeWidget->currentItem()->setText(ECOLUMN_AFTER_OPERATE, strRect);
	}
}
void FormConfig::roomOperateFinish(int formType, QString room)
{
	if (formType == OPERATE_BEFOR)
	{
		ui.treeWidget->currentItem()->setText(ECOLUMN_BEFORE_OPERATE, room);

	}
	else if (formType == OPERATE_AFTER)
	{
		ui.treeWidget->currentItem()->setText(ECOLUMN_AFTER_OPERATE, room);
	}
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
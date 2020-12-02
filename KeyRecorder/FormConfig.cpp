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

#include "constant.h"
#include "ImageMacher.h"
#include "KeyInfo.h"
#include "Utils.h"

FormConfig::FormConfig(QWidget *parent)
	: QWidget(parent , Qt::Dialog)
	, m_bRecord(false)
{
	ui.setupUi(this);

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

	if (m_bRecord)
	{
		m_recordBegin = m_recordEnd;
		m_recordEnd = m_recordPos;
		m_recordPos = QPoint(x, y);
	}
	else {
		QTime current = QTime::currentTime();
		int interval = 0;
		if (!m_LastClickTime.isNull())
		{
			interval = m_LastClickTime.msecsTo(current);
		}

		m_vecPoint.push_back(KeyInfo(m_vecPoint.size(), x, y, interval, m_vecPoint.size() + 1, 0, QString("")));
		m_LastClickTime = current;
	}
}

void FormConfig::on_pushButtonStart_clicked()
{
	m_vecPoint.clear();
	m_pMouseHook->setHook();
}

void FormConfig::on_pushButtonStop_clicked()
{
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

}
void FormConfig::on_pushButtonRecoText_clicked()
{

}
void FormConfig::on_pushButtonRom_clicked()
{

}
void FormConfig::on_pushButtonPMS_clicked()
{

}

void FormConfig::on_pushButtonTest_clicked()
{
	QRect rect(m_recordBegin, m_recordEnd);
	qDebug() << "record rect:" << rect;
	QByteArray bytes;
	m_pScreenGraber->grabScreen(rect.x(), rect.y(), rect.width(), rect.height(), bytes);

	bool ok = false;
	int nThreshold = ui.lineEditThreshold->text().toInt(&ok);
	if (!ok) {
		nThreshold = 100;
	}

	QByteArray grayBytes;
	ImageMacher macher;
	macher.grayImage(bytes, grayBytes, nThreshold);

	QString strResult = m_pRecongnizer->DoRecongnize(grayBytes.data(), grayBytes.count());

	ui.labelResult->setText(strResult);
}


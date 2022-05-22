#include "StepCfg.h"
#include "constant.h"
#include "Configure.h"
#include <QFILE>
#include <QTextStream>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonArray>


void StepCfg::init()
{
	parseItemDetail();
	parseOrignalDatas(FILE_ORIDATA_CFG);


}
void StepCfg::getStepInfo()
{}
bool StepCfg::parseItemDetail()
{
	QFile file(FILE_ITEM_CFG);
	if (!file.open(QFile::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	//QTextStream in(&file);
	//QString strData = in.readAll();
	//QByteArray data = strData.toLocal8Bit();

	QByteArray data = file.readAll();
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
	if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
		if (!doucment.isArray()) {
			file.close();
			return false;
		}
		QJsonArray arr = doucment.array();
		for (int i = 0; i < arr.size(); i++)
		{
			QJsonObject object = arr[i].toObject();

			ItemDetail item;
			item.key = Configure::getString(object, QString("key"));
			item.type = Configure::getInt(object, QString("type"));

			QJsonArray popups = object.value("popup").toArray();
			for (int j = 0; j < popups.size(); j++)
			{
				item.popupItems.append(popups[j].toString());
			}
			m_vecItemDetail.append(item);
			m_MapItemDetail.insert(item.key, item);
		}
	}

	file.close();
	return true;
}

bool StepCfg::parseOrignalDatas(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	//QTextStream in(&file);
	//QString strData = in.readAll();
	//QByteArray data = strData.toLocal8Bit();

	QByteArray data = file.readAll();

	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
	if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
		if (!doucment.isArray()) {
			file.close();
			return false;
		}
		QJsonArray arr = doucment.array();
		for (int i = 0; i < arr.size(); i++)
		{
			QJsonObject object = arr[i].toObject(); 
			Household household;
			parseHousehold(object, household);
			m_vecOrignalData.append(household);
		}
	}

	file.close();
	return true;
}

Household StepCfg::parseHousehold(QJsonObject object, Household& household)
{
	QJsonValue base = object.value("base");
	QJsonObject objectBase = base.toObject();
	parseContent(objectBase, household.m_mapBaseData);

	QJsonValue value = object.value("people");
	QJsonArray arr = value.toArray();
	for (int i = 0; i < arr.size(); i++)
	{
		QJsonObject object = arr[i].toObject(); 
		QMap<QString, Content> mapContent;
		parseContent(object, mapContent);
		household.m_vecPepleData.push_back(mapContent);
	}

	QJsonValue detect= object.value("detect");
	QJsonObject objectDet = detect.toObject();
	parseContent(objectDet, household.m_mapDetectData);

	return household;
}
QMap<QString, Content> StepCfg::parseContent(QJsonObject object, QMap<QString, Content>& mapContent)
{
	QStringList keys = object.keys();
	for (int iKey = 0; iKey < keys.size(); iKey++)
	{
		QString key = keys[iKey];
		QString value = Configure::getString(object, key);

		QMap<QString, ItemDetail>::iterator itrDetail = m_MapItemDetail.find(key);
		if (itrDetail == m_MapItemDetail.end())
		{
			continue;
		}
		ItemDetail detail = itrDetail.value();
		Content content;
		content.key = key;
		content.text = value;
		content.type = detail.type;
		content.popupIndex = detail.popupItems.indexOf(value);
		content.needScroll = false;

		mapContent.insert(key, content);
	}

	return mapContent;
}

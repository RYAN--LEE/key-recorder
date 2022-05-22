#pragma once

#include "KeyInfo.h"
#include <QVector>
#include <QMap>
#include <QStringList>


class Content
{
public:
	QString key;
	QString text;
	int type;
	int popupIndex;
	bool needScroll;
};

class Household
{
public:
	QMap<QString, Content> m_mapBaseData;
	QVector<QMap<QString, Content> > m_vecPepleData;
	QMap<QString, Content> m_mapDetectData;
};

enum EOperate
{
	E_OPERATE_CLICK = 0,
	E_OPERATE_INPUT = 1,
	E_OPERATE_POPUP = 2,
};
class ItemDetail
{
public:
	QString key;
	int type;
	QStringList popupItems;
};



class StepCfg
{
public:
	static StepCfg* instance()
	{
		static StepCfg instance;
		return &instance;
	}

	QVector<ItemDetail> getItemDetails()
	{
		return m_vecItemDetail;
	}
	QMap<QString, ItemDetail> getItemDetailsMap()
	{
		return m_MapItemDetail;
	}

	QVector<Household> getContentMaps()
	{
		return m_vecOrignalData;
	}

	void init();
	void getStepInfo();
	bool parseItemDetail();

	bool parseSteps(QString fileName);
	bool parseOrignalDatas(QString fileName);

private:
	Household parseHousehold(QJsonObject object, Household& household);
	QMap<QString, Content> parseContent(QJsonObject object, QMap<QString, Content>& mapContent);
public:
	QVector<Household> m_vecOrignalData;
	//QList<QMap<QString, Content> >  m_vecOrignalData;
	QMap<QString, ItemDetail> m_MapItemDetail;

	QVector<ItemDetail> m_vecItemDetail;
};


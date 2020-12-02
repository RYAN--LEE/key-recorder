#include "Utils.h"
#include <QStringList>

//text_1-2-3-4  x-y-width-length
QString Utils::encodeTextMatch(QRect rect)
{
	return "text_" + QString::number(rect.x()) + "-" + QString::number(rect.y()) + "-" +
		QString::number(rect.width()) + "-" + QString::number(rect.height());
}

QRect Utils::decodeTextMatch(QString text)
{
	QString strRect = text.right(text.size() - 5);
	QStringList strList = strRect.split("-");
	if (strList.size() == 4)
	{
		return QRect(strList[0].toInt(), strList[1].toInt(), strList[2].toInt(), strList[3].toInt());
	}

	return QRect();
}
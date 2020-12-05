#pragma once
#include <QString>
#include <QRect>

class Utils
{
public:
	static QString encodeTextMatch(QRect rect);
	static QRect decodeTextMatch(QString text);
};


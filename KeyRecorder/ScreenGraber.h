#pragma once

#include <QObject>

class ScreenGraber : public QObject
{
	Q_OBJECT

public:
	ScreenGraber(QObject *parent);
	~ScreenGraber();

	void grabScreen(int x, int y, int width, int height, QByteArray& bytes);
};

#include "ScreenGraber.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QBuffer>

ScreenGraber::ScreenGraber(QObject *parent)
	: QObject(parent)
{
}

ScreenGraber::~ScreenGraber()
{
}

void ScreenGraber::grabScreen(int x, int y, int width, int height, QByteArray& bytes)
{
	QScreen* screen = QGuiApplication::primaryScreen();
	QPixmap img = screen->grabWindow(0, x, y, width, height);

	//img.save("test.jpg", "jpg");
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	img.save(&buffer, "jpg");
}

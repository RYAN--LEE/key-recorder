#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

class WidgetTitle : public QWidget
{
	Q_OBJECT

protected:

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

public:
	WidgetTitle(QWidget *parent);
	~WidgetTitle();

private:
	bool   m_mousePressed;
	QPoint m_offset;
};

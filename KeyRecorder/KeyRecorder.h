#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyRecorder.h"
#include <QEvent>
#include <QVector>
#include <QPoint>
#include "MouseHook.h"
#include "FormMoreInfo.h"


class KeyRecorder : public QMainWindow
{
	Q_OBJECT

public:
	KeyRecorder(QWidget* parent = Q_NULLPTR);
	~KeyRecorder() {};

signals:
	void startPlay(QVector<QPoint> points);

public slots:
	void recieveClicked(long x, long y);
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonPlay_clicked();
	void on_pushButtonMore_clicked();
	void playClick(QVector<QPoint> points);

private:
	bool save(QVector<QPoint>& vecPoint, int begin, int end);

private:
	Ui::KeyRecorderClass	ui;
	MouseHook*				m_pMouseHook;
	QVector<QPoint>			m_vecPoint;

	long					m_nPlayInterval;
	FormMoreInfo* m_pFormMoreInfo;
};

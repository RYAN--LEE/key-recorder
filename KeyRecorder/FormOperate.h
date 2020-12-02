#pragma once

#include <QDialog>
#include "ui_FormOperate.h"
#include <QString>
#include <QRect>

enum OperateType {
	ImageMatch,
	TextMatch,
	QueryRoom,
};
class FormOperate : public QDialog
{
	Q_OBJECT

public:
	FormOperate(QWidget *parent = Q_NULLPTR);
	~FormOperate();

signals:
	void operateImageMatch(QString name);
	void operateTextMatch(QRect rect);
	void operateRoom(QString room);

public slots:
	void on_pushButtonImg_clicked();
	void on_pushButtonText_clicked();
	void on_pushButtonRom_clicked();

	void captureFinished(QPixmap catureImage,QRect rect);


private:
	Ui::FormOperate ui;

	int m_nOperateType;
};

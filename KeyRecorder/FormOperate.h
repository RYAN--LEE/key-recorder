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

enum FormType {
	OPERATE_BEFOR,
	OPERATE_AFTER,
};
class FormOperate : public QDialog
{
	Q_OBJECT

public:
	FormOperate(QWidget *parent = Q_NULLPTR);
	~FormOperate();

	void setType(int nType);

signals:
	void operateImageMatch(int formType, QString name, QRect rect);
	void operateTextMatch(int formType, QRect rect);
	void operateRoom(int formType, QString room);

public slots:
	void on_pushButtonImg_clicked();
	void on_pushButtonText_clicked();
	void on_pushButtonRom_clicked();

	void captureFinished(QPixmap catureImage, QRect rect);
	void captureCancel();

	void showCapture();


private:
	Ui::FormOperate ui;

	int m_nOperateType;
	int m_nFormType;
};

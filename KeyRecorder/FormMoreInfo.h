#pragma once

#include <QDialog>
#include "ui_FormMoreInfo.h"

class FormMoreInfo : public QDialog
{
	Q_OBJECT

public:
	FormMoreInfo(QWidget *parent = Q_NULLPTR);
	~FormMoreInfo();

private:
	Ui::FormMoreInfo ui;
};

#pragma once

#include <QWidget>
#include "ui_FormMoreInfo.h"

class FormMoreInfo : public QWidget
{
	Q_OBJECT

public:
	FormMoreInfo(QWidget *parent = Q_NULLPTR);
	~FormMoreInfo();

private:
	Ui::FormMoreInfo ui;
};

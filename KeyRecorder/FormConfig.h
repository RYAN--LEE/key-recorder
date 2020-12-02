#pragma once

#include <QWidget>
#include "ui_FormConfig.h"

class FormConfig : public QWidget
{
	Q_OBJECT

public:
	FormConfig(QWidget *parent = Q_NULLPTR);
	~FormConfig();

private:
	Ui::FormConfig ui;
};

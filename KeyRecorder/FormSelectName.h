#pragma once

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include "ui_FormSelectName.h"

class FormSelectName : public QWidget
{
	Q_OBJECT

public:
	FormSelectName(QWidget *parent, QString prefix);
	~FormSelectName();

protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
signals:
	void selectName(QString name);
public slots:
	void on_pushButtonOK_clicked();
	void on_pushButtoncancel_clicked();
	void on_pushButtonClose_clicked();

private:
	Ui::FormSelectName ui;

	bool m_bLeftBtnClk;
	QPoint m_Press;
	QPoint m_Move;
	QString m_Prefix;
};

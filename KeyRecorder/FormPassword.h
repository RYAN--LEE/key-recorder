#pragma once

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include "ui_FormPassword.h"

class FormPassword : public QWidget
{
	Q_OBJECT

public:
	FormPassword(QWidget *parent = Q_NULLPTR);
	~FormPassword();
protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
signals:
	void verifySuccess();
	void verifyFailed();
public slots:
	void on_pushButtonOK_clicked();
	void on_pushButtoncancel_clicked();
	void on_pushButtonClose_clicked();

private:
	Ui::FormPassword ui;

	bool m_bLeftBtnClk;
	QPoint m_Press;
	QPoint m_Move;
};

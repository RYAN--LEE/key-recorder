#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include "ui_FormAbort.h"

class FormAbort : public QWidget
{
	Q_OBJECT

public:
	FormAbort(QWidget *parent = Q_NULLPTR);
	~FormAbort();

protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
public slots:
	void on_pushButtonOK_clicked();
	void on_pushButtonClose_clicked();

private:
	Ui::FormAbort ui;

	bool m_bLeftBtnClk;
	QPoint m_Press;
	QPoint m_Move;
};

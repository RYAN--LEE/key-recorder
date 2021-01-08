#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include "ui_FormInform.h"

class FormInform : public QWidget
{
	Q_OBJECT

public:
	FormInform(QString title, QString info, QWidget *parent = Q_NULLPTR);
	~FormInform();
	static void showInfo(QString title, QString info, QWidget* parent);
protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

public slots:
	void on_pushButtonOK_clicked();
	void on_pushButtonClose_clicked();

private:
	Ui::FormInform ui;

	bool m_bLeftBtnClk;
	QPoint m_Press;
	QPoint m_Move;
};

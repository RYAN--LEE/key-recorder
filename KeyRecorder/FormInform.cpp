#include "FormInform.h"
#include <QApplication>
#include <QDesktopWidget>

FormInform::FormInform(QString title, QString info, QWidget *parent)
	: QWidget(parent, Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
	, m_bLeftBtnClk(false)
{
	ui.setupUi(this);
	if (!title.isEmpty())
	{
		ui.labelTip->setText(title);
	}
	ui.labelTip->setText(info);
	ui.pushButtonOK->setFocus();
	ui.pushButtonOK->setDefault(true);
	setAttribute(Qt::WA_QuitOnClose, false);
}

FormInform::~FormInform()
{
}

void FormInform::showInfo(QString title, QString info, QWidget* parent)
{
	FormInform* form = new FormInform(title, info, parent);
	QDesktopWidget* deskdop = QApplication::desktop();
	form->move((deskdop->width() - form->width()) / 2, (deskdop->height() - form->height()) / 2);
	form->show();
}


void FormInform::on_pushButtonOK_clicked()
{
	this->close();
}

void FormInform::on_pushButtonClose_clicked()
{
	this->close();
}


void FormInform::mousePressEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton &&
		ui.widgetTitle->frameGeometry().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		m_bLeftBtnClk = true;
	}
	event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void FormInform::mouseReleaseEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton) {
		m_bLeftBtnClk = false;
	}
	event->ignore();
}

void FormInform::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bLeftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}
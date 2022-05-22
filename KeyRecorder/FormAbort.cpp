#include "FormAbort.h"

FormAbort::FormAbort(QWidget *parent)
	: QWidget(parent, Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
	, m_bLeftBtnClk(false)
{
	ui.setupUi(this);

	ui.pushButtonOK->setFocus();
	ui.pushButtonOK->setDefault(true);

	ui.labelTip->setText(QString::fromLocal8Bit("防返贫监测系统智能辅助机器人\n\n版本：v1.0.0"));
	setAttribute(Qt::WA_QuitOnClose, false);
}

FormAbort::~FormAbort()
{
}


void FormAbort::on_pushButtonOK_clicked()
{
	this->close();
}

void FormAbort::on_pushButtonClose_clicked()
{
	this->close();
}


void FormAbort::mousePressEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton &&
		ui.widgetTitle->frameGeometry().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		m_bLeftBtnClk = true;
	}
	event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void FormAbort::mouseReleaseEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton) {
		m_bLeftBtnClk = false;
	}
	event->ignore();
}

void FormAbort::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bLeftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}

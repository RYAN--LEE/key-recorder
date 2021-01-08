#include "FormPassword.h"

const QString PASSWORD = "202117";
FormPassword::FormPassword(QWidget *parent)
	: QWidget(parent, Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
	, m_bLeftBtnClk(false)
{
	ui.setupUi(this);

	ui.lineEditPwd->setFocus();

	ui.pushButtonOK->setShortcut(Qt::Key_Return);
}

FormPassword::~FormPassword()
{
}

void FormPassword::on_pushButtonOK_clicked()
{
	if (ui.lineEditPwd->text() != PASSWORD)
	{
		ui.labelState->setText(QString::fromLocal8Bit("�������"));
		return;
	}

	emit verifySuccess();
	this->close();
}

void FormPassword::on_pushButtonClose_clicked()
{
	emit verifyFailed();
	this->close();
}
void FormPassword::on_pushButtoncancel_clicked()
{
	emit verifyFailed();
	this->close();
}

void FormPassword::mousePressEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton &&
		ui.widgetTitle->frameGeometry().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		m_bLeftBtnClk = true;
	}
	event->ignore();//��ʾ�������´����¼��������Ŀؼ�������ȥ��ȡ
}

void FormPassword::mouseReleaseEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton) {
		m_bLeftBtnClk = false;
	}
	event->ignore();
}

void FormPassword::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bLeftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}
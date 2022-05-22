#include "FormSelectName.h"
#include "StepCfg.h"

FormSelectName::FormSelectName(QWidget* parent, QString prefix)
	: QWidget(parent, Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
	, m_bLeftBtnClk(false)
	, m_Prefix(prefix)
{
	ui.setupUi(this);

	ui.comboBox->setFocus();

	ui.pushButtonOK->setShortcut(Qt::Key_Return);
	setAttribute(Qt::WA_QuitOnClose, false);

	QStringList list;
	StepCfg *cfg = StepCfg::instance();
	QVector<ItemDetail> items = cfg->getItemDetails();
	for (QVector<ItemDetail>::Iterator i = items.begin(); i != items.end(); i++)
	{
		list.append(i->key);
	}
	ui.comboBox->addItems(list);
}

FormSelectName::~FormSelectName()
{
}

void FormSelectName::on_pushButtonOK_clicked()
{
	QString data = ui.comboBox->currentText();

	emit selectName(m_Prefix + data);
	this->close();
}

void FormSelectName::on_pushButtonClose_clicked()
{
	this->close();
}
void FormSelectName::on_pushButtoncancel_clicked()
{
	this->close();
}

void FormSelectName::mousePressEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton &&
		ui.widgetTitle->frameGeometry().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		m_bLeftBtnClk = true;
	}
	event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void FormSelectName::mouseReleaseEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton) {
		m_bLeftBtnClk = false;
	}
	event->ignore();
}

void FormSelectName::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bLeftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}
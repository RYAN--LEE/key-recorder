#include "FormOperate.h"
#include "FormCaptureScreen.h"
#include "constant.h"
#include <QDateTime>
#include <QTimer>

FormOperate::FormOperate(QWidget *parent)
	: QDialog(parent)
	, m_nFormType(OPERATE_AFTER)
{
	ui.setupUi(this);
}

FormOperate::~FormOperate()
{
}

void FormOperate::setType(int nType)
{
	m_nFormType = nType;
	if (m_nFormType == OPERATE_AFTER)
	{
		ui.pushButtonText->show();
		ui.pushButtonRom->show();
		ui.pushButtonCard->show();
	}
	else
	{
		ui.pushButtonText->hide();
		ui.pushButtonRom->hide();
		ui.pushButtonCard->hide();
	}
}
void FormOperate::on_pushButtonImg_clicked()
{
	this->parentWidget()->hide();
	this->hide();
	m_nOperateType = ImageMatch;

	QTimer::singleShot(600, this, SLOT(showCapture()));
}

void FormOperate::showCapture()
{
	FormCaptureScreen* pForm = new FormCaptureScreen(NULL);
	connect(pForm, &FormCaptureScreen::signalCompleteCature, this, &FormOperate::captureFinished);
	connect(pForm, &FormCaptureScreen::signalCancelCature, this, &FormOperate::captureCancel);
	pForm->show();
}

void FormOperate::on_pushButtonText_clicked()
{
	this->parentWidget()->hide();
	this->hide();
	m_nOperateType = TextMatch;
	QTimer::singleShot(600, this, SLOT(showCapture()));
}

void FormOperate::on_pushButtonRom_clicked()
{
	this->hide();
	emit operateRoom(m_nFormType, "room_get");
}

void FormOperate::on_pushButtonCard_clicked()
{
	this->hide();
	emit operateCreateCard(m_nFormType, "create_card");
}

void FormOperate::captureFinished(QPixmap catureImage, QRect rect)
{
	this->parentWidget()->show();

	switch (m_nOperateType)
	{
		case ImageMatch:
		{
			this->hide();
			QString name = "img_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
			QString path = IMG_DIR + name;
			catureImage.save(path, "png");
			emit operateImageMatch(m_nFormType, name, rect);

			break;
		}
		case TextMatch:
		{
			this->hide();
			emit operateTextMatch(m_nFormType, rect);
			break;
		}
		case QueryRoom:
		{
			this->hide();
			break;
		}
		case CreateCard:
		{
			this->hide();
			break;
		}
		default:
		{
			break;
		}
	};
}

void FormOperate::captureCancel()
{
	this->hide();
	this->parentWidget()->show();
}

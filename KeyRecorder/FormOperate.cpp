#include "FormOperate.h"
#include "FormCaptureScreen.h"
#include "constant.h"
#include <QDateTime>
#include <QTimer>

FormOperate::FormOperate(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

FormOperate::~FormOperate()
{
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
	emit operateRoom("room_get");
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
			emit operateImageMatch(name, rect);

			break;
		}
		case TextMatch:
		{
			this->hide();
			emit operateTextMatch(rect);
			break;
		}
		case QueryRoom:
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

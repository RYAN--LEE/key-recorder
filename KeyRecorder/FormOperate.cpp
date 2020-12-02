#include "FormOperate.h"
#include "FormCaptureScreen.h"
#include <QDateTime>

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
	m_nOperateType = ImageMatch;
	FormCaptureScreen* pForm = new FormCaptureScreen(NULL);
	connect(pForm, &FormCaptureScreen::signalCompleteCature, this, &FormOperate::captureFinished);
	pForm->show();
}
void FormOperate::on_pushButtonText_clicked()
{
	m_nOperateType = TextMatch;
	FormCaptureScreen* pForm = new FormCaptureScreen(NULL);
	connect(pForm, &FormCaptureScreen::signalCompleteCature, this, &FormOperate::captureFinished);
	pForm->show();
}
void FormOperate::on_pushButtonRom_clicked()
{
	emit operateRoom("room_get");
}

void FormOperate::captureFinished(QPixmap catureImage, QRect rect)
{
	switch (m_nOperateType)
	{
		case ImageMatch:
		{
			this->hide();
			QString name = "img_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
			QString path = "./" + name;
			catureImage.save(path, "png");
			emit operateImageMatch(name);
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

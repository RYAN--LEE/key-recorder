#include "FormCaptureScreen.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>


FormCaptureScreen::FormCaptureScreen(QWidget* parent)
    : QWidget(parent)
    , m_currentCaptureState(initCapture)
{
    m_pOK = new QPushButton(this);
    connect(m_pOK, &QPushButton::clicked, this, &FormCaptureScreen::captureEnd);
    m_pOK->setText(QString::fromLocal8Bit("ȷ��"));
    m_pOK->hide();

    m_pCancel = new QPushButton(this);
    connect(m_pCancel, &QPushButton::clicked, this, &FormCaptureScreen::captureCancel);
    m_pCancel->setText(QString::fromLocal8Bit("ȡ��"));
    m_pCancel->hide();
    // ui.setupUi(this);
    initWindow();
    loadBackgroundPixmap();

}

FormCaptureScreen::~FormCaptureScreen()
{

}

void FormCaptureScreen::initWindow()
{
    this->setMouseTracking(true);
    // ���ڴ�������QQ��������ʼ����ʾ����㣬�������� Qt::WindowStaysOnTopHint;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
    this->setMouseTracking(true);
}

void FormCaptureScreen::loadBackgroundPixmap()
{
    m_loadPixmap = QPixmap::grabWindow(QApplication::desktop()->winId()); //ץȡ��ǰ��Ļ��ͼƬ;
    m_screenwidth = m_loadPixmap.width();
    m_screenheight = m_loadPixmap.height();
}

void FormCaptureScreen::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_currentCaptureState == initCapture)
    {
        m_currentCaptureState = beginCaptureImage;
        m_beginPoint = event->pos();

        m_pOK->setGeometry(event->pos().x(), event->pos().y(), 30, 30);
        m_pOK->show();

        m_pCancel->setGeometry(event->pos().x() + 30, event->pos().y(), 30, 30);
        m_pCancel->show();
    }
    else if (event->button() == Qt::LeftButton && isPressPointInSelectRect(event->pos()))
    {
        m_currentCaptureState = beginMoveCaptureArea;
        setCursor(Qt::SizeAllCursor);
        m_beginMovePoint = event->pos();
    }

    return QWidget::mousePressEvent(event);
}

void FormCaptureScreen::mouseMoveEvent(QMouseEvent* event)
{
    if (m_currentCaptureState == beginCaptureImage)
    {
        m_endPoint = event->pos();
        update();
        m_pOK->setGeometry(event->pos().x(), event->pos().y(), 30, 30);
        m_pOK->show();

        m_pCancel->setGeometry(event->pos().x() + 30, event->pos().y(), 30, 30);
        m_pCancel->show();
    }
    else if (m_currentCaptureState == beginMoveCaptureArea)
    {
        m_endMovePoint = event->pos();

        update();
    }

    // ��������Ƿ���ѡ�����������������ʽ;
    if (isPressPointInSelectRect(event->pos()))
    {
        setCursor(Qt::SizeAllCursor);
    }
    else if (!isPressPointInSelectRect(event->pos()) && m_currentCaptureState != beginMoveCaptureArea)
    {
        setCursor(Qt::ArrowCursor);
    }

    return QWidget::mouseMoveEvent(event);
}

void FormCaptureScreen::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_currentCaptureState == beginCaptureImage)
    {
        m_currentCaptureState = finishCaptureImage;
        m_endPoint = event->pos();
        update();
    }
    else if (m_currentCaptureState == beginMoveCaptureArea)
    {
        m_currentCaptureState = finishMoveCaptureArea;
        m_endMovePoint = event->pos();
        update();
    }
    return QWidget::mouseReleaseEvent(event);
}

// ��ǰ��������Ƿ���ѡȡ�ľ���������;
bool FormCaptureScreen::isPressPointInSelectRect(QPoint mousePressPoint)
{
    QRect selectRect = getRect(m_beginPoint, m_endPoint);
    if (selectRect.contains(mousePressPoint))
    {
        return true;
    }

    return false;
}

void FormCaptureScreen::paintEvent(QPaintEvent* event)
{
    m_painter.begin(this);          //�����ػ�;

    QColor shadowColor = QColor(0, 0, 0, 100);                      //��Ӱ��ɫ����;
    m_painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));    //���û���;
    m_painter.drawPixmap(0, 0, m_loadPixmap);                       //������ͼƬ����������;
    m_painter.fillRect(m_loadPixmap.rect(), shadowColor);           //��Ӱ��Ч��;

    switch (m_currentCaptureState)
    {
    case initCapture:
        break;
    case beginCaptureImage:
    case finishCaptureImage:
    case beginMoveCaptureArea:
    case finishMoveCaptureArea:
        m_currentSelectRect = getSelectRect();
        drawCaptureImage();
    default:
        break;
    }

    m_painter.end();  //�ػ����;
}

// ���ݵ�ǰ��ȡ״̬��ȡ��ǰѡ�еĽ�ͼ����;
QRect FormCaptureScreen::getSelectRect()
{
    if (m_currentCaptureState == beginCaptureImage || m_currentCaptureState == finishCaptureImage)
    {
        return getRect(m_beginPoint, m_endPoint);
    }
    else if (m_currentCaptureState == beginMoveCaptureArea || m_currentCaptureState == finishMoveCaptureArea)
    {
        return getMoveRect();
    }

    return QRect(0, 0, 0, 0);
}

// ���Ƶ�ǰѡ�еĽ�ͼ����;
void FormCaptureScreen::drawCaptureImage()
{
    m_capturePixmap = m_loadPixmap.copy(m_currentSelectRect);
    m_painter.drawPixmap(m_currentSelectRect.topLeft(), m_capturePixmap);
    m_painter.drawRect(m_currentSelectRect);
}

void FormCaptureScreen::keyPressEvent(QKeyEvent* event)
{
    // Esc ���˳���ͼ;
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
    // Eeter����ɽ�ͼ;
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        signalCompleteCature(m_capturePixmap, m_currentSelectRect);
        close();
    }
}

void FormCaptureScreen::captureEnd()
{
    signalCompleteCature(m_capturePixmap, m_currentSelectRect);
    close();
}

void FormCaptureScreen::captureCancel()
{
    emit signalCancelCature();
    close();
}

// ����beginPoint , endPoint ��ȡ��ǰѡ�еľ���
QRect FormCaptureScreen::getRect(const QPoint& beginPoint, const QPoint& endPoint)
{
    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);
    // �������Ϊ��ʱ������ͼ����;
    // ���Կ�QQ��ͼ����ѡȡ��ͼ����Ϊ��ʱĬ��Ϊ2;
    if (selectedRect.width() == 0)
    {
        selectedRect.setWidth(1);
    }
    if (selectedRect.height() == 0)
    {
        selectedRect.setHeight(1);
    }

    return selectedRect;
}

// ��ȡ�ƶ���,��ǰѡ�еľ���;
QRect FormCaptureScreen::getMoveRect()
{
    // ͨ��getMovePoint�����ȼ�鵱ǰ�Ƿ��ƶ�������Ļ;
    QPoint movePoint = getMovePoint();
    QPoint beginPoint = m_beginPoint + movePoint;
    QPoint endPoint = m_endPoint + movePoint;
    // �����ƶ�ѡ��ʱ���µ�ǰm_beginPoint , m_endPoint,��ֹ��һ�β���ʱ��ȡ��ͼƬ������;
    if (m_currentCaptureState == finishMoveCaptureArea)
    {
        m_beginPoint = beginPoint;
        m_endPoint = endPoint;
        m_beginMovePoint = QPoint(0, 0);
        m_endMovePoint = QPoint(0, 0);
    }
    return getRect(beginPoint, endPoint);
}

QPoint FormCaptureScreen::getMovePoint()
{
    QPoint movePoint = m_endMovePoint - m_beginMovePoint;
    QRect currentRect = getRect(m_beginPoint, m_endPoint);
    // ��鵱ǰ�Ƿ��ƶ�������Ļ;

    //�ƶ�ѡ���Ƿ񳬳���Ļ��߽�;
    if (currentRect.topLeft().x() + movePoint.x() < 0)
    {
        movePoint.setX(0 - currentRect.topLeft().x());
    }
    //�ƶ�ѡ���Ƿ񳬳���Ļ�ϱ߽�;
    if (currentRect.topLeft().y() + movePoint.y() < 0)
    {
        movePoint.setY(0 - currentRect.topLeft().y());
    }
    //�ƶ�ѡ���Ƿ񳬳���Ļ�ұ߽�;
    if (currentRect.bottomRight().x() + movePoint.x() > m_screenwidth)
    {
        movePoint.setX(m_screenwidth - currentRect.bottomRight().x());
    }
    //�ƶ�ѡ���Ƿ񳬳���Ļ�±߽�;
    if (currentRect.bottomRight().y() + movePoint.y() > m_screenheight)
    {
        movePoint.setY(m_screenheight - currentRect.bottomRight().y());
    }

    return movePoint;
}
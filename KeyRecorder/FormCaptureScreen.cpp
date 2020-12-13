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
    m_pOK->setText(QString::fromLocal8Bit("确定"));
    m_pOK->hide();

    m_pCancel = new QPushButton(this);
    connect(m_pCancel, &QPushButton::clicked, this, &FormCaptureScreen::captureCancel);
    m_pCancel->setText(QString::fromLocal8Bit("取消"));
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
    // 由于存在类似QQ这样界面始终显示在最顶层，设置属性 Qt::WindowStaysOnTopHint;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
    this->setMouseTracking(true);
}

void FormCaptureScreen::loadBackgroundPixmap()
{
    m_loadPixmap = QPixmap::grabWindow(QApplication::desktop()->winId()); //抓取当前屏幕的图片;
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

    // 根据鼠标是否在选中区域内设置鼠标样式;
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

// 当前鼠标坐标是否在选取的矩形区域内;
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
    m_painter.begin(this);          //进行重绘;

    QColor shadowColor = QColor(0, 0, 0, 100);                      //阴影颜色设置;
    m_painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));    //设置画笔;
    m_painter.drawPixmap(0, 0, m_loadPixmap);                       //将背景图片画到窗体上;
    m_painter.fillRect(m_loadPixmap.rect(), shadowColor);           //画影罩效果;

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

    m_painter.end();  //重绘结束;
}

// 根据当前截取状态获取当前选中的截图区域;
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

// 绘制当前选中的截图区域;
void FormCaptureScreen::drawCaptureImage()
{
    m_capturePixmap = m_loadPixmap.copy(m_currentSelectRect);
    m_painter.drawPixmap(m_currentSelectRect.topLeft(), m_capturePixmap);
    m_painter.drawRect(m_currentSelectRect);
}

void FormCaptureScreen::keyPressEvent(QKeyEvent* event)
{
    // Esc 键退出截图;
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
    // Eeter键完成截图;
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

// 根据beginPoint , endPoint 获取当前选中的矩形
QRect FormCaptureScreen::getRect(const QPoint& beginPoint, const QPoint& endPoint)
{
    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);
    // 避免宽或高为零时拷贝截图有误;
    // 可以看QQ截图，当选取截图宽或高为零时默认为2;
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

// 获取移动后,当前选中的矩形;
QRect FormCaptureScreen::getMoveRect()
{
    // 通过getMovePoint方法先检查当前是否移动超出屏幕;
    QPoint movePoint = getMovePoint();
    QPoint beginPoint = m_beginPoint + movePoint;
    QPoint endPoint = m_endPoint + movePoint;
    // 结束移动选区时更新当前m_beginPoint , m_endPoint,防止下一次操作时截取的图片有问题;
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
    // 检查当前是否移动超出屏幕;

    //移动选区是否超出屏幕左边界;
    if (currentRect.topLeft().x() + movePoint.x() < 0)
    {
        movePoint.setX(0 - currentRect.topLeft().x());
    }
    //移动选区是否超出屏幕上边界;
    if (currentRect.topLeft().y() + movePoint.y() < 0)
    {
        movePoint.setY(0 - currentRect.topLeft().y());
    }
    //移动选区是否超出屏幕右边界;
    if (currentRect.bottomRight().x() + movePoint.x() > m_screenwidth)
    {
        movePoint.setX(m_screenwidth - currentRect.bottomRight().x());
    }
    //移动选区是否超出屏幕下边界;
    if (currentRect.bottomRight().y() + movePoint.y() > m_screenheight)
    {
        movePoint.setY(m_screenheight - currentRect.bottomRight().y());
    }

    return movePoint;
}
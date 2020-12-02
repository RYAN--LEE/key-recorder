#pragma once

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include "ui_FormCaptureScreen.h"

enum state
{
    initCapture = 0,
    beginCaptureImage,
    finishCaptureImage,
    beginMoveCaptureArea,
    finishMoveCaptureArea,
};
class FormCaptureScreen : public QWidget
{
	Q_OBJECT

public:
	FormCaptureScreen(QWidget *parent = Q_NULLPTR);
	~FormCaptureScreen();

private slots:
    void captureEnd();
signals:
    void signalCompleteCature(QPixmap catureImage, QRect captureRect);

private:
    void initWindow();
    void loadBackgroundPixmap();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent* event);

    bool isPressPointInSelectRect(QPoint mousePressPoint);
    QRect getSelectRect();
    QRect getMoveRect();
    QRect getRect(const QPoint& beginPoint, const QPoint& endPoint);
    QPoint getMovePoint();
    void drawCaptureImage();


private:
    bool m_isMousePress;
    QPixmap m_loadPixmap;
    QPixmap m_capturePixmap;
    int m_screenwidth;
    int m_screenheight;
    QPoint m_beginPoint;
    QPoint m_endPoint;
    QPainter m_painter;

    int m_currentCaptureState;
    QPoint m_beginMovePoint;
    QPoint m_endMovePoint;
    QRect m_currentSelectRect;

    QPushButton* m_pOK;

private:
	Ui::FormCaptureScreen ui;
};

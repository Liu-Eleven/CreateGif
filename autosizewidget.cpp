/******************************************************************
 Copyright (C) 2018 - All Rights Reserved by
 文 件 名 : autosizewidget.cpp --- AutoSizeWidget
 作 者    : 倪又华
 编写日期 : 2018
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2018/12/10
*******************************************************************/
#include "autosizewidget.h"
#include <QMouseEvent>

AutoSizeWidget::AutoSizeWidget(QWidget *parent)
    : QWidget(parent)
{
    isLeftPressDown = false;
    dir = NONE;

    // 追踪鼠标
    this->setMouseTracking(true);
}

AutoSizeWidget::~AutoSizeWidget()
{

}

void AutoSizeWidget::judgeRegionSetCursor(const QPoint& currentPoint)
{
    // 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = currentPoint.x();
    int y = currentPoint.y();

    if ((tl.x() + Padding) >= x && tl.x() <= x && (tl.y() + Padding) >= y && tl.y() <= y)
    {
        // 左上角
        dir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    }
    else if ((x >= rb.x() - Padding) && x <= rb.x() && y >= (rb.y() - Padding) && y <= rb.y())
    {
        // 右下角
        dir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if (x <= (tl.x() + Padding) && x >= tl.x() && (y >= rb.y() - Padding) && y <= rb.y())
    {
        //左下角
        dir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (x <= rb.x() && x >= rb.x() - Padding && y >= tl.y() && y <= tl.y() + Padding) {
        // 右上角
        dir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (x <= (tl.x() + Padding) && x >= tl.x()) {
        // 左边
        dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (x <= rb.x() && x >= (rb.x() - Padding)) {
        // 右边
        dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (y >= tl.y() && y <= tl.y() + Padding) {
        // 上边
        dir = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if (y <= rb.y() && y >= (rb.y() - Padding)) {
        // 下边
        dir = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else
    {
        // 默认
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void AutoSizeWidget::mouseReleaseEvent(QMouseEvent *event)
{
#if 0
    if (event->button() == Qt::LeftButton) {
    isLeftPressDown = false;
        if (dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
#else
    isLeftPressDown = false;
    dir = NONE;
    this->releaseMouse();
    this->setCursor(QCursor(Qt::ArrowCursor));
#endif

    QWidget::mouseReleaseEvent(event);
}

void AutoSizeWidget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        isLeftPressDown = true;
        if (dir != NONE) {
            this->mouseGrabber();
        }
        else {
            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        }
        break;
    default:
        QWidget::mousePressEvent(event);
    }
}

void AutoSizeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if (!isLeftPressDown)
    {
        this->judgeRegionSetCursor(gloPoint);
    }
    else
    {
        if (dir != NONE)
        {
            QRect rMove(tl, rb);

            switch (dir) {
            case LEFT:
                if (rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if (rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if (rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if (rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:
                QWidget::mouseMoveEvent(event);
                return;
                break;
            }
            this->setGeometry(rMove);
        }
        else {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
    }

    QWidget::mouseMoveEvent(event);
}


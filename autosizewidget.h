/******************************************************************
 Copyright (C) 2018 - All Rights Reserved by
 文 件 名 : autosizewidget.h --- AutoSizeWidget
 作 者    : 倪又华
 编写日期 : 2018
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2018/12/10
*******************************************************************/
#ifndef AUTOSIZEWIDGET_H
#define AUTOSIZEWIDGET_H

#include <QWidget>

enum Direction {
    UP, DOWN, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE
};

/**
 * @brief The AutoSizeWidget class
 * 代码来自网络：https://blog.csdn.net/wjh_init/article/details/79244222
 * 感谢作者的分享
 */
class AutoSizeWidget : public QWidget
{
    Q_OBJECT
public:
    AutoSizeWidget(QWidget *parent=0);
    ~AutoSizeWidget();

protected:
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
private:
    void judgeRegionSetCursor(const QPoint&);
    const int Padding = 1;
    // 判断左键是否按下
    bool isLeftPressDown;
    // 窗口移动拖动时需要记住的点
    QPoint dragPosition;
    // 窗口大小改变时，记录改变方向
    Direction dir;
};

#endif // AUTOSIZEWIDGET_H

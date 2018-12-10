#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include <gif.h>
#include <QImage>
#include <QTimer>

namespace Ui {
class RecordWidget;
}

class RecordWidget : public QWidget{
    Q_OBJECT

public:
    explicit RecordWidget(QWidget *parent = nullptr);
    ~RecordWidget();

private slots:
    void SaveImage();
    // 启动/停止
    void on_btnRecord_clicked();
    void on_btnStop_clicked();
    void on_btnClose_clicked();
    // 修改录制框大小
    void on_lineEditWidth_editingFinished();
    void on_lineEditHeight_editingFinished();

private:
    Ui::RecordWidget *ui;
    QTimer timer;
    // 当前屏幕，用于截取当前像素
    QScreen  *m_screen;
    QString   m_strFileName;

    // gif
    Gif_H m_Gif;
    Gif_H::GifWriter *m_gifWriter = nullptr;
    // 100为1s
    uint  m_Fps = 33;

protected:
    QRect  m_rectRecord;
    QPoint mousePoint;
    bool   m_mousePressed;

    void paintEvent(QPaintEvent *);
    // 重构鼠标事件，实现拖动
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // WIDGET_H

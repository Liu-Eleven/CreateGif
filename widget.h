#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include <gif.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTimer timer;
    //
    Gif_H m_Gif;
    Gif_H::GifWriter *m_GifWriter = nullptr;
    uint  m_Fps = 33;//100为1s
};

#endif // WIDGET_H

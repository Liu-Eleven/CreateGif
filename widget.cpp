#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(width(),height());

    auto saveImage=[&]{
        if(!m_GifWriter)return;
        auto image = grab().toImage().convertToFormat(QImage::Format_RGBA8888);
        m_Gif.GifWriteFrame(m_GifWriter, image.bits(), image.width(), image.height(), m_Fps);
    };
    connect(&timer,&QTimer::timeout,saveImage);

    connect(ui->pushButton,&QPushButton::clicked,[&](bool checked){
        if(checked){
            auto file = QTime::currentTime().toString("hh-mm-ss.gif");
            if(m_GifWriter)delete m_GifWriter;
            m_GifWriter = new Gif_H::GifWriter;
            if(!m_Gif.GifBegin(m_GifWriter,file.toLocal8Bit().data(),width(),height(), m_Fps)){
                delete m_GifWriter;
                m_GifWriter = nullptr;
                return;
            }
            saveImage();
            timer.start(m_Fps*10);
        }else{
            timer.stop();
            m_Gif.GifEnd(m_GifWriter);
            delete m_GifWriter;
            m_GifWriter = nullptr;
        }
    });
}

Widget::~Widget(){
    delete ui;
}

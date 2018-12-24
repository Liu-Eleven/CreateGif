#include "recordwidget.h"
#include "ui_recordwidget.h"

#include <QDateTime>
#include <QApplication>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QDebug>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QScreen>

RecordWidget::RecordWidget(QWidget *parent) :
    AutoSizeWidget(parent),
    ui(new Ui::RecordWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
//    ui->btnClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    m_screen = QApplication::primaryScreen();
    connect(&timer, SIGNAL(timeout()), this, SLOT(SaveImage()));
}

RecordWidget::~RecordWidget()
{
    if (nullptr != m_gifWriter)
    {
        delete m_gifWriter;
        m_gifWriter = nullptr;
    }
    delete ui;
}

void RecordWidget::on_btnRecord_clicked()
{
    m_Fps = ui->lineEditFps->text().toInt();
    m_strFileName = QDateTime::currentDateTime().toString("Gif_yyyyMMddhhmmss.gif");
    if (nullptr != m_gifWriter)
    {
        delete m_gifWriter;
        m_gifWriter = nullptr;
    }

    int nW = ui->lineEditWidth->text().toInt();
    int nH = ui->lineEditHeight->text().toInt();
    m_gifWriter = new Gif_H::GifWriter;
    bool bOk = m_Gif.GifBegin(m_gifWriter, m_strFileName.toLocal8Bit().data(), nW, nH, m_Fps);
    if (!bOk)
    {
        delete m_gifWriter;
        m_gifWriter = nullptr;
        return;
    }

    SaveImage();
    timer.start(m_Fps * 10);
    ui->labelStatus->setText("开始录制...");
}

void RecordWidget::on_btnStop_clicked()
{
    if (!m_gifWriter) return;
    timer.stop();
    m_Gif.GifEnd(m_gifWriter);

    delete m_gifWriter;
    m_gifWriter = nullptr;

    ui->labelStatus->setText("录制完成：" + m_strFileName);

#if 0
    // 打开文件
    QDesktopServices::openUrl(QUrl(m_strFileName));
#endif
}

/**
 * @brief RecordWidget::SaveImage
 * 保存图像
 */
void RecordWidget::SaveImage()
{
    if (!m_gifWriter) return;

    QPixmap  pix = m_screen->grabWindow(0, this->x() + m_rectRecord.x(),
                                        this->y() + m_rectRecord.y(),
                                        m_rectRecord.width(), m_rectRecord.height());
    QImage image = pix.toImage().convertToFormat(QImage::Format_RGBA8888);;
    m_Gif.GifWriteFrame(m_gifWriter, image.bits(), m_rectRecord.width(), m_rectRecord.height(), m_Fps);
}

void RecordWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor("#67b5a2"));
    int nW = ui->lineEditWidth->text().toInt();
    int nH = ui->lineEditHeight->text().toInt();

    painter.setCompositionMode( QPainter::CompositionMode_Clear );
    m_rectRecord = QRect(2, 32, nW, nH);
    painter.fillRect(m_rectRecord, Qt::SolidPattern );
}


#if 0
void RecordWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mousePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void RecordWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void RecordWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}
#else
void RecordWidget::resizeEvent(QResizeEvent *event)
{
    int nW = event->size().width() - 4;
    ui->lineEditWidth->setText(QString::number(nW));
    int nH = event->size().height() - 78;
    ui->lineEditHeight->setText(QString::number(nH));
}
#endif

void RecordWidget::on_btnClose_clicked()
{
    qApp->quit();
}

/**
 * @brief RecordWidget::on_lineEditWidth_editingFinished
 */
void RecordWidget::on_lineEditWidth_editingFinished()
{
    int nW = ui->lineEditWidth->text().toInt();
    int nH = ui->lineEditHeight->text().toInt();
    this->resize(nW + 4, nH + 78);
}

/**
 * @brief RecordWidget::on_lineEditHeight_editingFinished
 * 修改录制框的高度
 */
void RecordWidget::on_lineEditHeight_editingFinished()
{
    int nW = ui->lineEditWidth->text().toInt();
    int nH = ui->lineEditHeight->text().toInt();
    this->resize(nW + 4, nH + 78);
}

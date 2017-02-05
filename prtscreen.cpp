#include "prtscreen.h"
#include <QDebug>

PrtScreen::PrtScreen(QWidget *parent)
    : QWidget(parent)
{
    isDrawing = false;

    QScreen *screen = QGuiApplication::primaryScreen();
    fullDesktop = screen->grabWindow(QApplication::desktop()->winId()).toImage();
}

void PrtScreen::setBackgroundImage()
{
    int red, green, blue;
    int width = fullDesktop.width(), height = fullDesktop.height();
    QImage fullTempPix = QImage(width, height, QImage::Format_RGB32);
    for (int i=0; i<width; i++) {
        for (int j=0; j<height; j++) {
            red = qRed(fullDesktop.pixel(i, j)) * 0.6;
            green = qGreen(fullDesktop.pixel(i, j)) * 0.6;
            blue = qBlue(fullDesktop.pixel(i, j)) * 0.6;
            fullTempPix.setPixel(i, j, qRgb(red, green, blue));
        }
    }

    QPalette   palette;
    palette.setBrush(this->backgroundRole(),QBrush(fullTempPix));
    this->setPalette(palette);
}

void PrtScreen::grapWindow()
{
    int width = abs(endPot.x() - startPot.x());
    int height = rubberBand->height();
    qDebug() << height << endl;
    int x = startPot.x() < endPot.x() ? startPot.x() : endPot.x();
    int y = startPot.y() < endPot.y() ? startPot.y() : endPot.y();
    QImage prts = fullDesktop.copy(x, y, width, height);
    prts.save(tr("desktop.jpg", "jpg"));
}


void PrtScreen::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        startPot = event->pos();
        if (!rubberBand) {
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        }
        rubberBand->show();
        rubberBand->setGeometry(startPot.x(), startPot.y(), 0, 0);
    }
    QWidget::mousePressEvent(event);
}

void PrtScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        if (rubberBand) {
            endPot = event->pos();
            rubberBand->setGeometry(QRect(startPot, endPot).normalized());
        }
    }
}

void PrtScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        grapWindow();
        this->close();
    }
    QWidget::keyPressEvent(event);
}

//void PrtScreen::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    painter.drawPixmap(0, 0, fullTempPix);
//    QWidget::paintEvent(event);
//}

void PrtScreen::showEvent(QShowEvent *event)
{
    showFullScreen();
    setCursor(Qt::CrossCursor);
    setBackgroundImage();
}

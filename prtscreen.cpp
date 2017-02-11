#include "prtscreen.h"
#include <QDebug>
#include <QDir>
#include <QMainWindow>

PrtScreen::PrtScreen(QWidget *parent, QScreen *screen)
    : QDialog(parent),
      mainScreen(screen)
{
    rubberBand = NULL;

    setWindowFlags(Qt::Window);
//    QScreen *screen = QGuiApplication::primaryScreen();
//    QMainWindow *object = qobject_cast<QMainWindow*>(this->parent());

    fullDesktop = mainScreen->grabWindow(0).toImage();
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

void PrtScreen::grabWindow()
{
    int width = abs(endPot.x() - startPot.x());
    int height = rubberBand->height();
//    qDebug() << height << endl;
    int x = startPot.x() < endPot.x() ? startPot.x() : endPot.x();
    int y = startPot.y() < endPot.y() ? startPot.y() : endPot.y();
    QImage prts = fullDesktop.copy(x, y, width, height);
    QString dir = QDir::currentPath();
    qDebug() << dir << endl;
//    if (prts.save(dir + tr("/Desktop.jpg"), "jpg"))
//        qDebug() << "1" << endl;
    emit grabFinised(prts);
}


void PrtScreen::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
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
        grabWindow();
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

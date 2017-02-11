#ifndef PRTSCREEN_H
#define PRTSCREEN_H

#include <QWidget>
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QRubberBand>
#include <QPixmap>
#include <QImage>
#include <QDialog>

class PrtScreen : public QDialog
{
    Q_OBJECT
public:
    explicit PrtScreen(QWidget *parent = 0, QScreen *screen = QGuiApplication::primaryScreen());

signals:
    void grabFinised(QImage image);

private:
    void setBackgroundImage();
    void grabWindow();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
//    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

private:
    QScreen *mainScreen;
    QPoint startPot;
    QPoint endPot;
    QImage fullDesktop;
    QRubberBand *rubberBand;
};

#endif // PRTSCREEN_H

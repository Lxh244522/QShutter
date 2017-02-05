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

class PrtScreen : public QWidget
{
    Q_OBJECT
public:
    explicit PrtScreen(QWidget *parent = 0);

signals:

private:
    void setBackgroundImage();
    void grapWindow();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
//    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

private:
    bool isDrawing;
    QPoint startPot;
    QPoint endPot;
    QImage fullDesktop;
    QRubberBand *rubberBand;
};

#endif // PRTSCREEN_H

#ifndef QSHUTTER_H
#define QSHUTTER_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QtWidgets>

class QShutter : public QMainWindow
{
    Q_OBJECT

public:
    QShutter(QWidget *parent = 0);
    ~QShutter();

private:
    void createMenu();
    void initTrayIcon();

private slots:
    void selectRang();
    void grapWindows();
    void scaleImage();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

private:
    int scrWidth;
    int scrHeight;
    int m_bPressed;
    QImage image;
    QSize preSize;
    QLabel *labelImage;
    QScrollArea *scrollArea;
    QPoint m_point;
    QSystemTrayIcon *trayIcon;
    QMenuBar *mainMenuBar;
};

#endif // QSHUTTER_H

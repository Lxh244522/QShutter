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
    void grabDesktop();

private slots:
    void selectGrabWindows();
    void newGrab();
    void newDesktop();
    void scaleImage();
    void setLabelImage(QImage image);
    void updateStatusBar();
    void updateCheckBox();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *);

private:
    int scrWidth;
    int scrHeight;
    QImage image;
    QLabel *imageLabel;
    QLabel *statusLabel;
    QSpinBox *delaySpinBox;
    QScrollArea *scrollArea;
    QSystemTrayIcon *trayIcon;
    QMenuBar *mainMenuBar;
    QCheckBox *hideThisWindowCheckBox;
    QStatusBar *mainStatusBar;
};

#endif // QSHUTTER_H

#include "qshutter.h"
#include "prtscreen.h"
#include <QTextEdit>
#include <QDebug>

QShutter::QShutter(QWidget *parent)
    : QMainWindow(parent)
{
//    setCentralWidget();
    m_bPressed = false;

    mainMenuBar = new QMenuBar();
    setMenuBar(mainMenuBar);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, false);
//    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    createMenu();
//    initTrayIcon();
}

QShutter::~QShutter()
{

}

void QShutter::createMenu()
{
    QMenu *fileMenu = mainMenuBar->addMenu(tr("&File"));

    QToolBar *reToolBar = addToolBar(tr("refresh"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    fileToolBar->setMovable(false);
    fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *reloadAction = new QAction(this);
    reloadAction->setIcon(QIcon(":/Resource/Icon/24x24/refresh.png"));
    reloadAction->setIconText(tr(""));

    QFont font;
    font.setPixelSize(13);
//    font.setWeight(QFont::Normal);

    QAction *selectAction = new QAction(this);
    selectAction->setIcon(QIcon(":/Resource/Icon/24x24/cursor.png"));
    selectAction->setIconText(tr("Selection"));
    selectAction->setFont(font);
    connect(selectAction, &QAction::triggered, this, &QShutter::grapWindows);

    QAction *desktopAction = new QAction(this);
    desktopAction->setIcon(QIcon(":/Resource/Icon/24x24/desktop.png"));
    desktopAction->setIconText(tr("Desktop"));
    desktopAction->setFont(font);

    fileMenu->addAction(selectAction);
    reToolBar->addAction(reloadAction);
    reToolBar->setMovable(false);
//    fileToolBar->addAction(reloadAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(selectAction);
    fileToolBar->addAction(desktopAction);
//    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

//    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

//    QMenu *screenshotMenu = menuBar()->addMenu(tr("&ScreenShot"));

//    QMenu *goMenu = menuBar()->addMenu(tr("&Go"));

    //    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
}

void QShutter::initTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/shutter.png"), this);
    QMenu *trayIconMenu = new QMenu;
    QAction *quitAction = new QAction(tr("Quit"), this);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

    connect(quitAction, &QAction::triggered, this, &QMainWindow::close);
}

void QShutter::selectRang()
{
    QPixmap pixmap = QWidget::grab(QRect(QPoint(0, 0), QSize(this->width(), this->height())));
    if (pixmap.save("widget.png", "png"))
        qDebug() << "1" << endl;
}

void QShutter::grapWindows()
{
    PrtScreen *fullScreenWidget = new PrtScreen();
    fullScreenWidget->show();
}

// The relative position of mouse to window (event->globalPos() - this->pos()
void QShutter::mousePressEvent(QMouseEvent *event)
{

}

void QShutter::mouseReleaseEvent(QMouseEvent *event)
{

}

void QShutter::mouseMoveEvent(QMouseEvent *event)
{

}

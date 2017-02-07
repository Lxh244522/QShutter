#include "qshutter.h"
#include "prtscreen.h"
#include <QTextEdit>
#include <QDebug>

QShutter::QShutter(QWidget *parent)
    : QMainWindow(parent)
{
//    setCentralWidget();
    m_bPressed = false;

    labelImage = new QLabel;
//    label->setText(tr("111"));
    labelImage->setBackgroundRole(QPalette::Base);
    labelImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    labelImage->setScaledContents(true);

    image.load(":/Resource/Image/Desktop.jpg");
    labelImage->setPixmap(QPixmap::fromImage(image));
    labelImage->setMinimumSize(175, 80);
//    labelImage->resize(300, 150);

    QString message = tr("%1 x %2, Size: %3").arg(image.width()).arg(image.height()).arg(image.byteCount());
    statusBar()->showMessage(message);

    scrollArea = new QScrollArea;
    QPalette palette;
    palette.setBrush(scrollArea->backgroundRole(), QBrush(QColor(237, 237, 237)));
    scrollArea->setPalette(palette);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(labelImage);
    scrollArea->setMinimumHeight(85);
    scrollArea->setMinimumWidth(300);
    scrollArea->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    scrollArea->setVisible(true);

    preSize = image.size();

    setCentralWidget(scrollArea);

    mainMenuBar = new QMenuBar();
    setMenuBar(mainMenuBar);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, false);
    createMenu();
//    initTrayIcon();
}

QShutter::~QShutter()
{

}

void QShutter::createMenu()
{
    QMenu *fileMenu = mainMenuBar->addMenu(tr("&File"));

//    QToolBar *reToolBar = addToolBar(tr("refresh"));
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
//    reToolBar->addAction(reloadAction);
//    reToolBar->setMovable(false);
    fileToolBar->setIconSize(QSize(24, 24));
    fileToolBar->addAction(reloadAction);
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
    PrtScreen *fullScreenWidget = new PrtScreen(this);
    fullScreenWidget->show();
}

void QShutter::scaleImage()
{
    QSize imageSize = image.size();
    QSize centerSize = labelImage->size();
    if (imageSize.width() > centerSize.width() &&
            imageSize.height() > centerSize.height()) {
        double imageWidth = imageSize.width();
        double widgetWidth = centerSize.width();
        labelImage->resize(imageSize * (widgetWidth / imageWidth));
    }
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


void QShutter::resizeEvent(QResizeEvent *event)
{
    QSize imageLabelSize = labelImage->size();
    QSize scrollSize = scrollArea->size();
    double scale = (double)image.width() / (double)image.height();
    if (imageLabelSize.width() > scrollSize.width()) {
        labelImage->resize(scrollSize.width(), scrollSize.width() / scale);
    }
    if (imageLabelSize.height() > scrollSize.height()) {
        labelImage->resize(scrollSize.height() * scale, scrollSize.height());
    }
    if (event->oldSize().width() < event->size().width() && event->oldSize().height() < event->size().height()) {
        if (labelImage->size().width() < image.size().width() &&
                labelImage->size().height() < image.size().height()) {
            int width = scrollSize.width();
            int height = scrollSize.height();
            if (height * scale <= scrollSize.width()) {
                labelImage->resize(height * scale, height);
            } else {
                labelImage->resize(width, width / scale);
            }
        }
    }
    if (event->oldSize().width() < event->size().width() && event->oldSize().height() == event->size().height()) {
        if (imageLabelSize.width() < scrollSize.width() && imageLabelSize.height() < scrollSize.height()) {
            if (labelImage->size().width() < image.size().width() &&
                    labelImage->size().height() < image.size().height()) {
                labelImage->resize(scrollSize.width(), scrollSize.width() / scale);
            }
        }
    }
    if (event->oldSize().height() < event->size().height() && event->oldSize().width() == event->size().width()) {
        if (imageLabelSize.height() < scrollSize.height() && imageLabelSize.width() < scrollSize.width()) {
            if (labelImage->size().width() < image.size().width() &&
                    labelImage->size().height() < image.size().height()) {
                labelImage->resize(scrollSize.height() * scale, scrollSize.height());
            }
        }
    }
}

void QShutter::showEvent(QShowEvent *event)
{
}

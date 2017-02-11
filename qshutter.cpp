#include "qshutter.h"
#include "prtscreen.h"
#include <QTextEdit>
#include <QDebug>

QShutter::QShutter(QWidget *parent)
    : QMainWindow(parent)
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->setMinimumSize(175, 80);

    QPalette palette;
    palette.setBrush(imageLabel->backgroundRole(), QBrush(QColor(237, 237, 237)));

    mainStatusBar = new QStatusBar();
    mainStatusBar->setFixedHeight(30);

    statusLabel = new QLabel(tr(""), this);
    hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"), this);
    delaySpinBox = new QSpinBox(this);
    delaySpinBox->setSuffix(tr(" s"));
    delaySpinBox->setMaximum(60);
    delaySpinBox->setValue(5);
    QLabel *tip = new QLabel(tr("Delay: "), this);


    typedef void (QSpinBox::*QSpinIntSignal) (int);
    connect(delaySpinBox, static_cast<QSpinIntSignal>(&QSpinBox::valueChanged),
            this, &QShutter::updateCheckBox);

    scrollArea = new QScrollArea;
    palette.setBrush(scrollArea->backgroundRole(), QBrush(QColor(237, 237, 237)));
    scrollArea->setPalette(palette);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(imageLabel);
    scrollArea->setMinimumHeight(85);
    scrollArea->setMinimumWidth(300);
    scrollArea->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    scrollArea->setVisible(true);

    QWidget *widget = new QWidget(this);

    setCentralWidget(widget);

    QHBoxLayout *statusBarLayout = new QHBoxLayout;
    statusBarLayout->addWidget(statusLabel);
    statusBarLayout->addStretch();
    statusBarLayout->addWidget(hideThisWindowCheckBox);
    statusBarLayout->addWidget(tip);
    statusBarLayout->addWidget(delaySpinBox);
    statusBarLayout->setContentsMargins(3, 4, 30, 4);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(statusBarLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    widget->setLayout(mainLayout);

    mainMenuBar = new QMenuBar();
    setMenuBar(mainMenuBar);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, false);
    createMenu();
}

QShutter::~QShutter()
{

}

void QShutter::createMenu()
{
    QMenu *fileMenu = mainMenuBar->addMenu(tr("&File"));

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
    connect(selectAction, &QAction::triggered, this, &QShutter::newGrab);

    QAction *desktopAction = new QAction(this);
    desktopAction->setIcon(QIcon(":/Resource/Icon/24x24/desktop.png"));
    desktopAction->setIconText(tr("Desktop"));
    desktopAction->setFont(font);
    connect(desktopAction, &QAction::triggered, this, &QShutter::newDesktop);

    fileMenu->addAction(selectAction);
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

void QShutter::selectGrabWindows()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = this->windowHandle())
        screen = window->screen();
    if (!screen)
        return;

    PrtScreen *fullScreenWidget = new PrtScreen(this, screen);
    connect(fullScreenWidget, &PrtScreen::grabFinised, this, &QShutter::setLabelImage);
    fullScreenWidget->show();
    if (hideThisWindowCheckBox->isChecked())
        show();
}

void QShutter::newGrab()
{
    if (hideThisWindowCheckBox->isChecked())
        hide();
    QTimer::singleShot(1000, this, &QShutter::selectGrabWindows);
}

void QShutter::newDesktop()
{
    if (hideThisWindowCheckBox->isChecked())
        hide();
    QTimer::singleShot(delaySpinBox->value() * 1000, this, &QShutter::grabDesktop);
}

void QShutter::scaleImage()
{
    QSize imageSize = image.size();
    QSize centerSize = imageLabel->size();
    if (imageSize.width() > centerSize.width() &&
            imageSize.height() > centerSize.height()) {
        double imageWidth = imageSize.width();
        double widgetWidth = centerSize.width();
        imageLabel->resize(imageSize * (widgetWidth / imageWidth));
    }
}

void QShutter::setLabelImage(QImage image)
{
    this->image = image;
    imageLabel->setPixmap(QPixmap::fromImage(this->image).
                          scaled(image.size(),
                                 Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation));

    if (!image.isNull()) {
        QSize imageSize = image.size();
        QSize scrollSize = scrollArea->size();
        double scale = (double)image.width() / (double)image.height();

        int width = scrollSize.width();
        int height = scrollSize.height();
        if (scrollSize.width() < imageSize.width() ||
                scrollSize.height() < imageSize.height()) {
            if (height * scale <= scrollSize.width()) {
                imageLabel->resize(height * scale, height);
            } else {
                imageLabel->resize(width, width / scale);
            }
        }
        QString message = tr("%1 x %2 pxiel Size: %3 KB")
                .arg(image.width())
                .arg(image.height())
                .arg((double)image.byteCount() / 1024);
        statusLabel->setText(message);
    }
}

void QShutter::grabDesktop()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = this->windowHandle())
        screen = window->screen();
    if (!screen)
        return;

    if (delaySpinBox->value() != 0)
        QApplication::beep();

    QImage prts = screen->grabWindow(QApplication::desktop()->winId()).toImage();
    setLabelImage(prts);
    if (hideThisWindowCheckBox->isChecked())
        show();
}

void QShutter::updateStatusBar()
{

}

void QShutter::updateCheckBox()
{
    if (delaySpinBox->value() == 0) {
        hideThisWindowCheckBox->setDisabled(true);
        hideThisWindowCheckBox->setChecked(false);
    } else {
        hideThisWindowCheckBox->setDisabled(false);
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
    if (!image.isNull()) {
        QSize imageLabelSize = imageLabel->size();
        QSize scrollSize = scrollArea->size();
        double scale = (double)image.width() / (double)image.height();
        if (imageLabelSize.width() > scrollSize.width()) {
            imageLabel->resize(scrollSize.width(), scrollSize.width() / scale);
        }
        if (imageLabelSize.height() > scrollSize.height()) {
            imageLabel->resize(scrollSize.height() * scale, scrollSize.height());
        }
        if (event->oldSize().width() < event->size().width() && event->oldSize().height() < event->size().height()) {
            if (imageLabel->size().width() < image.size().width() &&
                    imageLabel->size().height() < image.size().height()) {
                int width = scrollSize.width();
                int height = scrollSize.height();
                if (height * scale <= scrollSize.width()) {
                    imageLabel->resize(height * scale, height);
                } else {
                    imageLabel->resize(width, width / scale);
                }
            }
        }
        if (event->oldSize().width() < event->size().width() && event->oldSize().height() == event->size().height()) {
            if (imageLabelSize.width() < scrollSize.width() && imageLabelSize.height() < scrollSize.height()) {
                if (imageLabel->size().width() < image.size().width() &&
                        imageLabel->size().height() < image.size().height()) {
                    imageLabel->resize(scrollSize.width(), scrollSize.width() / scale);
                }
            }
        }
        if (event->oldSize().height() < event->size().height() && event->oldSize().width() == event->size().width()) {
            if (imageLabelSize.height() < scrollSize.height() && imageLabelSize.width() < scrollSize.width()) {
                if (imageLabel->size().width() < image.size().width() &&
                        imageLabel->size().height() < image.size().height()) {
                    imageLabel->resize(scrollSize.height() * scale, scrollSize.height());
                }
            }
        }
    }
}

void QShutter::showEvent(QShowEvent *)
{
//    if (!image.isNull()) {
//        qDebug() << "showevent" << endl;
//        QSize imageSize = image.size();
//        QSize scrollSize = scrollArea->size();
//        double scale = (double)image.width() / (double)image.height();

//        int width = scrollSize.width();
//        int height = scrollSize.height();
//        if (scrollSize.width() < imageSize.width() &&
//                scrollSize.height() < imageSize.height()) {
//            if (height * scale <= scrollSize.width()) {
//                labelImage->resize(height * scale, height);
//            } else {
//                labelImage->resize(width, width / scale);
//            }
//        }
//    }
}

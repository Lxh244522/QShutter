#include "qshutter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QShutter w;

    const QRect avGeometry = QApplication::desktop()->availableGeometry(&w);
    w.resize(825, 530);
    w.move((avGeometry.width() - w.width()) / 2,
           (avGeometry.height() - w.height()) / 2);
//    QFile style(":/Resource/Qss/style.qss");
//    style.open(QFile::ReadOnly);
//    qApp->setStyleSheet(style.readAll());
//    style.close();

    w.show();

    return a.exec();
}

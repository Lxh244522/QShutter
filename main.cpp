#include "qshutter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QShutter w;
    w.show();

    return a.exec();
}

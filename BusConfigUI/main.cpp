#include "BusConfigUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BusConfigUI w;
    w.show();
    return a.exec();
}

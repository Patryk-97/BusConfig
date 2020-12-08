#include "BusConfigUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   a.setWindowIcon(QIcon("icons/database.png"));
   BusConfigUI w;
   w.show();
   return a.exec();
}

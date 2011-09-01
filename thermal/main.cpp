#include <QtGui/QApplication>
#include "labcontrol.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LabControl w;
    w.show();

    return a.exec();
}

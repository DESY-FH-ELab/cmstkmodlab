#include <iostream>

#include <QtCore/QCoreApplication>
#include <QTimer>

#include "DefoDAQStreamReader.h"

int main(int argc, char *argv[])
{
  if (argc!=2) {
    std::cout << "usage: defoDAQ2ROOT <xml data file>" << std::endl;
    return -1;
  }

  QCoreApplication a(argc, argv);
  
  DefoDAQStreamReader *reader = new DefoDAQStreamReader(a.arguments(), &a);
  QObject::connect(reader, SIGNAL(finished()), &a, SLOT(quit()));
  
  QTimer::singleShot(0, reader, SLOT(run()));
  
  return a.exec();
}

#include <iostream>

#include <QtCore/QCoreApplication>
#include <QTimer>

#include "ThermoDAQStreamReader.h"

int main(int argc, char *argv[])
{
  if (argc!=3) {
    std::cout << "usage: thermoDAQ2ROOT <xml data file> <output file>" << std::endl;
    return -1;
  }

  QCoreApplication a(argc, argv);
  
  ThermoDAQStreamReader *reader = new ThermoDAQStreamReader(a.arguments(), &a);
  QObject::connect(reader, SIGNAL(finished()), &a, SLOT(quit()));
  
  QTimer::singleShot(0, reader, SLOT(run()));
  
  return a.exec();
}

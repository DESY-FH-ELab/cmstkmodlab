#include <iostream>

#include <QtCore/QCoreApplication>
#include <QTimer>

#include "ThermoDAQLogReader.h"

int main(int argc, char *argv[])
{
  if (argc!=2) {
    std::cout << "usage: thermoDAQ2Log <xml data file>" << std::endl;
    return -1;
  }

  QCoreApplication a(argc, argv);
  
  ThermoDAQLogReader *reader = new ThermoDAQLogReader(a.arguments(), &a);
  QObject::connect(reader, SIGNAL(finished()), &a, SLOT(quit()));
  
  QTimer::singleShot(0, reader, SLOT(run()));
  
  return a.exec();
}

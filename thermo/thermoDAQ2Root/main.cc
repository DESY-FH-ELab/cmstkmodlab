/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QtCore/QCoreApplication>
#include <QTimer>

#include "ThermoDAQStreamReader.h"
#include "ThermoDAQ2StreamReader.h"

int main(int argc, char *argv[])
{
  if (argc<3 || argc>4) {
    std::cout << "usage: thermoDAQ2ROOT <version> [options] <xml data file>" << std::endl;
    return -1;
  }

  QCoreApplication a(argc, argv);
  
  int fileversion = a.arguments().at(1).toInt();

  if (fileversion<1 || fileversion>2) {
    std::cout << "usage: thermoDAQ2ROOT [version] [options] <xml data file>" << std::endl;
    std::cout << "       version has to be either 1 or 2" << std::endl;
    return -1;
  }

  if (fileversion==1) {
    ThermoDAQStreamReader *reader = new ThermoDAQStreamReader(a.arguments(), &a);
    QObject::connect(reader, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, reader, SLOT(run()));
  }
  if (fileversion==2) {
    ThermoDAQ2StreamReader *reader = new ThermoDAQ2StreamReader(a.arguments(), &a);
    QObject::connect(reader, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, reader, SLOT(run()));
  }
  
  return a.exec();
}

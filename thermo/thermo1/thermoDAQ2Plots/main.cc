/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
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

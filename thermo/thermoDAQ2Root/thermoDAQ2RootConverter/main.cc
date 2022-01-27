/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
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

#include "ThermoDAQ2SlidingWindowConverter.h"

int main(int argc, char *argv[])
{
  if (argc!=5) {
    std::cout << "usage: thermoDAQ2ROOT <mode> <parameter> <input file> <output file>" << std::endl;
    std::cout << "  possible mode(s):" << std::endl;
    std::cout << "    slw              sliding window averager" << std::endl;
    std::cout << "      parameter(s)   size of window in seconds" << std::endl;
    return -1;
  }

  QCoreApplication a(argc, argv);

  QString mode = a.arguments().at(1);
  
  if (mode=="slw") {
    ThermoDAQ2SlidingWindowConverter *converter = new ThermoDAQ2SlidingWindowConverter(a.arguments(), &a);
    QObject::connect(converter, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, converter, SLOT(run()));
  }
  
  return a.exec();
}

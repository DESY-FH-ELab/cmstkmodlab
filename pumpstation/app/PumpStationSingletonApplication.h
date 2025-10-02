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

#ifndef PUMPSTATIONSINGLETONAPPLICATION_H
#define PUMPSTATIONSINGLETONAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>

class PumpStationSingletonApplication : public QApplication
{
  Q_OBJECT

public:
  PumpStationSingletonApplication(int &argc, char **argv,
                                  const char * uniqueKey = 0);
  ~PumpStationSingletonApplication();

  bool lock();

private:
  QSharedMemory* singular_;
};

#endif // PUMPSTATIONSINGLETONAPPLICATION_H

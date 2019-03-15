/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMODAQTHREAD_H
#define THERMODAQTHREAD_H

#include <QObject>
#include <QThread>

#include "ThermoDAQModel.h"

class ThermoDAQThread : public QThread
{
public:
  explicit ThermoDAQThread(ThermoDAQModel* daqModel,
                           QObject *parent = 0);
public:
  void run();

protected:
  ThermoDAQModel* daqModel_;
};

#endif // THERMODAQTHREAD_H

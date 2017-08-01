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

#include <QApplication>
#include <iostream>
#include <nqlogger.h>

#include "LaserThread.h"

LaserThread::LaserThread(QObject *parent)
    : QThread(parent)
{

}

LaserThread::~LaserThread()
{
}

void LaserThread::run()
{
  //    NQLog("LaserThread ", NQLog::Debug) << "run";
    exec();
}








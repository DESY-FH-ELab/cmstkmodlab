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

#ifndef LASERTHREAD_H
#define LASERTHREAD_H

#include <QThread>

class LaserThread : public QThread
{
 Q_OBJECT

 public:

  explicit LaserThread(QObject* parent=0);
  virtual ~LaserThread();

  void run();
};

#endif // LASERTHREAD_H                          

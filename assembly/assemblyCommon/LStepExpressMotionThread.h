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

#ifndef LSTEPEXPRESSMOTIONTHREAD_H
#define LSTEPEXPRESSMOTIONTHREAD_H

#include <LStepExpressMotionManager.h>

#include <QObject>
#include <QThread>

class LStepExpressMotionThread : public QThread
{
 Q_OBJECT

 public:

  explicit LStepExpressMotionThread(LStepExpressMotionManager*, QObject* parent=nullptr);
  virtual ~LStepExpressMotionThread();

  void run();

 protected:

  LStepExpressMotionManager* motion_manager_;
};

#endif // LSTEPEXPRESSMOTIONTHREAD_H

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

#ifndef OBJECTFINDERPATRECTHREAD_H
#define OBJECTFINDERPATRECTHREAD_H

#include <ObjectFinderPatRec.h>

#include <QObject>
#include <QThread>

class ObjectFinderPatRecThread : public QThread
{
 Q_OBJECT

 public:

  explicit ObjectFinderPatRecThread(ObjectFinderPatRec* finder, QObject* parent=0);
  virtual ~ObjectFinderPatRecThread();

  void run();

 protected:

  ObjectFinderPatRec* finder_;
};

#endif // OBJECTFINDERPATRECTHREAD_H

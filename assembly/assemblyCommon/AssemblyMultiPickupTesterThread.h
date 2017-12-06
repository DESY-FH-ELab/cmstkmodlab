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

#ifndef ASSEMBLYMULTIPICKUPTESTERTHREAD_H
#define ASSEMBLYMULTIPICKUPTESTERTHREAD_H

#include <AssemblyMultiPickupTester.h>

#include <QObject>
#include <QThread>

class AssemblyMultiPickupTesterThread : public QThread
{
 Q_OBJECT

 public:

  explicit AssemblyMultiPickupTesterThread(AssemblyMultiPickupTester*, QObject* parent=0);
  virtual ~AssemblyMultiPickupTesterThread();

  void run();

 protected:

  AssemblyMultiPickupTester* tester_;
};

#endif // ASSEMBLYMULTIPICKUPTESTERTHREAD_H

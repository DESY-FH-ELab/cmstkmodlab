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

#ifndef ASSEMBLYOBJECTFINDERPATRECTHREAD_H
#define ASSEMBLYOBJECTFINDERPATRECTHREAD_H

#include <AssemblyObjectFinderPatRec.h>

#include <QObject>
#include <QThread>

class AssemblyObjectFinderPatRecThread : public QThread
{
 Q_OBJECT

 public:

  explicit AssemblyObjectFinderPatRecThread(AssemblyObjectFinderPatRec* finder, QObject* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRecThread();

  void run();

 protected:

  AssemblyObjectFinderPatRec* finder_;
};

#endif // ASSEMBLYOBJECTFINDERPATRECTHREAD_H

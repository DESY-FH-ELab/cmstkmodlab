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

#ifndef MARKERFINDERPATRECTHREAD_H
#define MARKERFINDERPATRECTHREAD_H

#include <MarkerFinderPatRec.h>

#include <QObject>
#include <QThread>

class MarkerFinderPatRecThread : public QThread
{
 Q_OBJECT

 public:

  explicit MarkerFinderPatRecThread(MarkerFinderPatRec* finder, QObject* parent=0);

  virtual ~MarkerFinderPatRecThread();

  void run();

 protected:

  MarkerFinderPatRec* finder_;
};

#endif // MARKERFINDERPATRECTHREAD_H

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

#ifndef IMAGECONTROLLERTHREAD_H
#define IMAGECONTROLLERTHREAD_H

#include <ImageController.h>

#include <QObject>
#include <QThread>

class ImageControllerThread : public QThread
{
 Q_OBJECT

 public:

  explicit ImageControllerThread(ImageController*, QObject* parent=0);
  virtual ~ImageControllerThread();

  void run();

 protected:

  ImageController* imagec_;
};

#endif // IMAGECONTROLLERTHREAD_H

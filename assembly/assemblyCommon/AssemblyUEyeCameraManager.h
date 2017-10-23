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

#ifndef ASSEMBLYUEYECAMERAMANAGER_H
#define ASSEMBLYUEYECAMERAMANAGER_H

#include <AssemblyVUEyeCamera.h>

#include <QObject>

#include <opencv2/opencv.hpp>

class AssemblyUEyeCameraManager : public QObject
{
 Q_OBJECT

  public:

    explicit AssemblyUEyeCameraManager(AssemblyVUEyeCamera*, QObject* parent=0);
    virtual ~AssemblyUEyeCameraManager();

    AssemblyVUEyeCamera* camera() const { return camera_; }

  protected:

    AssemblyVUEyeCamera* camera_;

  signals:

//!!    void cameraOpened();
//!!    void cameraClosed();

    void  open_camera();
    void close_camera();

    void update_cameraInfo();

    void image_acquired(const cv::Mat&);

  public slots:

    virtual void open() {}
    virtual void close() {}

    virtual void acquire_image() {}
};

#endif // ASSEMBLYUEYECAMERAMANAGER_H

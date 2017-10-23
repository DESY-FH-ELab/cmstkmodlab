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

    bool model_connected() const { return model_connected_; }

  protected:

    AssemblyVUEyeCamera* camera_;

    bool model_connected_;

  signals:

    void camera_opened();
    void camera_closed();

    void  open_camera();
    void close_camera();

    void  call_camera();

    void image_acquired(const cv::Mat&);

  public slots:

    virtual void    connect_model();
    virtual void disconnect_model();

    virtual void open () { emit  open_camera(); }
    virtual void close() { emit close_camera(); }

    virtual void  enable_camera() { emit camera_opened(); }
    virtual void disable_camera() { emit camera_closed(); }

    virtual void acquire_image() { emit call_camera(); }

    void process_image(const cv::Mat& img){ emit image_acquired(img); }
};

#endif // ASSEMBLYUEYECAMERAMANAGER_H

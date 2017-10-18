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

#ifndef SNAPSHOTCONTROLLER_H
#define SNAPSHOTCONTROLLER_H

#include <QObject>

#include <opencv2/opencv.hpp>

class SnapshotController : public QObject
{
 Q_OBJECT

  public:

    explicit SnapshotController();
    virtual ~SnapshotController();

    bool is_enabled() const { return is_enabled_; }

  protected:
    bool is_enabled_;

  public slots:

    void enable();
    void disable();

    void acquireImage();

    void cameraOpened();
    void cameraClosed();

    void retrieveImage(const cv::Mat&);

  protected slots:

  signals:

    void openCamera  ();
    void closeCamera ();

    void getImage();

    void camera_is_opened();
    void camera_is_closed();

    void imageAcquired(const cv::Mat&);
};

#endif // SNAPSHOTCONTROLLER_H

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

#ifndef ASSEMBLYUEYESNAPSHOOTER_H
#define ASSEMBLYUEYESNAPSHOOTER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>

#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeView.h>

class AssemblyUEyeSnapShooter : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyUEyeSnapShooter(QWidget *parent = 0);

    void connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);

protected:

    void keyReleaseEvent(QKeyEvent *event);

    QScrollArea *scrollArea_;
    AssemblyUEyeView *imageView_;

    cv::Mat image_;

public slots:

    void snapShot();
    void imageAcquired(const cv::Mat&);
};

#endif // ASSEMBLYUEYEWIDGET_H

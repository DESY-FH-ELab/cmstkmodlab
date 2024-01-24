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

#ifndef ASSEMBLYUEYEVIEW_H
#define ASSEMBLYUEYEVIEW_H

#include <QLabel>
#include <QImage>
#include <QMutex>

#include <opencv2/opencv.hpp>

class AssemblyUEyeView : public QLabel
{
 Q_OBJECT

 public:

  explicit AssemblyUEyeView(QWidget* parent=nullptr);

  void    connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

  void setZoomFactor(float zoomFactor);

  void increaseZoomFactor();
  void decreaseZoomFactor();

 protected:

  void paintEvent(QPaintEvent*);

  QImage image_;
  float zoomFactor_;

  QMutex mutex_;

 public slots:

  void setImage(const cv::Mat&);
  void resetImage();
};

#endif // ASSEMBLYUEYEVIEW_H

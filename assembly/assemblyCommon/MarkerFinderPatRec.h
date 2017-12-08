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

#ifndef MARKERFINDERPATREC_H
#define MARKERFINDERPATREC_H

#include <nqlogger.h>

#include <QObject>
#include <QString>

#include <opencv2/opencv.hpp>

class MarkerFinderPatRec : public QObject
{
 Q_OBJECT

 public:

  explicit MarkerFinderPatRec(QObject* parent=0);
  virtual ~MarkerFinderPatRec();

  int  getGeneralThresholdValue()    { return generalThreshold_; }
  void setGeneralThresholdValue(const int v){ generalThreshold_ = v; }

  cv::Mat img;
  cv::Mat img_clip_A;
  cv::Mat img_clip_B;
  cv::Mat result_1;
  cv::Mat result_2;
  cv::Mat dst;

 protected:

  std::string cacheDirectory1_;
  std::string cacheDirectory2_;

  int generalThreshold_;

  double matchLoc_x_lab;
  double matchLoc_y_lab;

  int labmode_g;
  int objectmode_g;

 public slots:

  void setNewGeneralThreshold(const int, const cv::Mat&);
  void getCurrentGeneralThreshold();
  void updateThresholdImage(const cv::Mat&);

  void runObjectDetection(const int labmode, const int objectmode);
  void runObjectDetection_labmode(const cv::Mat&);

  virtual void findMarker_templateMatching(cv::Mat, cv::Mat);

 signals:

  void sendCurrentGeneralThreshold(const int);
  void sendUpdatedThresholdImage(const QString&);

  void locatePickupCorner_templateMatching(cv::Mat, cv::Mat);

  void reportObjectLocation(int, double, double, double);

  void updateImage(int, QString);
  void foundSensor(int);

  void getImage();
  void getImageBlur(cv::Mat, cv::Rect);

  void acquireImage();
};

#endif // MARKERFINDERPATREC_H

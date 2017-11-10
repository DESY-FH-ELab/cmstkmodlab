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

  explicit MarkerFinderPatRec(const QString&, const QString&, QObject* parent=0);
  virtual ~MarkerFinderPatRec();

  void set_threshold(const int);
  int  get_threshold() const { return threshold_; }

  void delete_image();
  void delete_binary_image();

 protected:

//!!  int mode_lab_;
//!!  int mode_obj_;

  int threshold_;
  int threshold_tpl_;

  cv::Mat image_mas_; // master image
  cv::Mat image_bin_; // master image + threshold
  cv::Mat image_tpl_; // template image

  bool updated_threshold_;
  bool updated_image_master_;
  bool updated_image_master_binary_;

  std::string cacheDirectory1_;
  std::string cacheDirectory2_;

  void PatRec(double&, cv::Point&, const cv::Mat&, const cv::Mat&, const double, const int) const;

 public slots:

  void update_threshold(const int);

  void acquire_image();

  void update_image(const cv::Mat&);
  void update_binary_image();

  cv::Mat get_binary_image(const cv::Mat&, const int) const;

  void run_PatRec(const int, const int);

  void template_matching(const cv::Mat&, const cv::Mat&, const cv::Mat&, const int);

//!!  void runObjectDetection(const int labmode, const int objectmode);
//!!  void runObjectDetection_labmode(const cv::Mat&);

 signals:

  void threshold_request();
  void threshold_updated();

  void image_request();

  void        image_updated(const cv::Mat&);
  void binary_image_updated(const cv::Mat&);

  void run_template_matching(const cv::Mat&, const cv::Mat&, const cv::Mat&, const int);




//!!  void locatePickupCorner_templateMatching(cv::Mat, cv::Mat);

  void reportObjectLocation(int, double, double, double);

  void updateImage(int, QString);
  void foundSensor(int);

  void getImage();
  void getImageBlur(cv::Mat, cv::Rect);

};

#endif // MARKERFINDERPATREC_H

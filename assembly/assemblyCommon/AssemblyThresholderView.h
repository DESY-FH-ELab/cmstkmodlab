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

#ifndef ASSEMBLYTHRESHOLDERVIEW_H
#define ASSEMBLYTHRESHOLDERVIEW_H

#include <AssemblyUEyeView.h>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>

#include <opencv2/opencv.hpp>

class AssemblyThresholderView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyThresholderView(QWidget* parent=nullptr);
  virtual ~AssemblyThresholderView() {}

  void    connectImageProducer_raw   (const QObject* sender, const char* signal);
  void disconnectImageProducer_raw   (const QObject* sender, const char* signal);

  void    connectImageProducer_binary(const QObject* sender, const char* signal);
  void disconnectImageProducer_binary(const QObject* sender, const char* signal);

 protected:

  void keyReleaseEvent(QKeyEvent*);

  // image raw
  cv::Mat imgraw_;

  AssemblyUEyeView* imgraw_ueye_;
  QScrollArea*      imgraw_scroll_;

  QPushButton* imgraw_load_button_;
  QPushButton* imgraw_save_button_;
  // ---------

  // image binary
  cv::Mat imgbin_;

  AssemblyUEyeView* imgbin_ueye_;
  QScrollArea*      imgbin_scroll_;

  QPushButton* imgbin_save_button_;

  QPushButton* imgbin_thresh_button_;
  QLabel*      imgbin_thresh_label_;
  QLineEdit*   imgbin_thresh_linee_;
  QSlider* threshold_slider_;

  QPushButton* imgbin_adathr_button_;
  QLabel*      imgbin_adathr_label_;
  QLineEdit*   imgbin_adathr_linee_;
  // ---------

 public slots:

  void load_image_raw();
  void save_image_raw();
  void save_image_binary();

  void update_image_raw   (const cv::Mat&);
  void update_image_binary(const cv::Mat&);

  void apply_threshold();
  void apply_adaptiveThreshold();

  void display_infoTab();

  void set_bw_threshold_slider(int);

 signals:

  void image_raw_request();
  void image_raw_updated(const cv::Mat&);
  void image_raw_updated();

  void image_binary_request();
  void image_binary_updated(const cv::Mat&);
  void image_binary_updated();

  void loaded_image_raw(const cv::Mat&);

  void threshold_request(const int);
  void adaptiveThreshold_request(const int);
};

#endif // ASSEMBLYTHRESHOLDERVIEW_H

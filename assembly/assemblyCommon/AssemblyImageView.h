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

#ifndef ASSEMBLYIMAGEVIEW_H
#define ASSEMBLYIMAGEVIEW_H

#include <AssemblyUEyeView.h>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QToolTip>
#include <QEventLoop>
#include <QProgressBar>

#include <opencv2/opencv.hpp>

class AssemblyImageView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyImageView(QWidget* parent=nullptr);
  virtual ~AssemblyImageView() {}

  void    connectImageProducer_image    (const QObject* sender, const char* signal);
  void disconnectImageProducer_image    (const QObject* sender, const char* signal);

  void    connectImageProducer_autofocus(const QObject* sender, const char* signal);
  void disconnectImageProducer_autofocus(const QObject* sender, const char* signal);

  QPushButton* autofocus_button() const { return autofocus_exe_button_; }
  QPushButton* autofocus_emergencyStop_button() const { return autofocus_stop_button_; }

 protected:

  void keyReleaseEvent(QKeyEvent*);

  // image
  AssemblyUEyeView* img_ueye_;
  QScrollArea*      img_scroll_;

  QPushButton* img_load_button_;
  QPushButton* img_save_button_;
  QPushButton* img_celi_button_;
  QPushButton* img_axes_button_;
  QPushButton* info_button;

  cv::Mat image_;
  cv::Mat image_raw_;

  bool image_modified_;
  // -------------------

  // auto-focusing
  AssemblyUEyeView *autofocus_ueye_;
  QScrollArea      *autofocus_scroll_;
  QLineEdit        *autofocus_result_bestZ_lineed_;
  QPushButton      *autofocus_exe_button_;
  QPushButton      *autofocus_stop_button_;
  QLineEdit        *autofocus_param_maxDZ_lineed_;
  QLineEdit        *autofocus_param_Nstep_lineed_;
  QPushButton      *autofocus_save_zscan_button_;
  QProgressBar* progBar_;

  cv::Mat image_zscan_;

  double mm_per_pixel_row_;
  double mm_per_pixel_col_;
  double angle_FromCameraXYtoRefFrameXY_deg_;
  // -------------------

 public slots:

  void update_image(const cv::Mat&, const bool update_image_raw=true);

  void load_image();
  void save_image();
  void modify_image_centerlines();
  void modify_image_axesConventions();

  void update_text(const double);

  void   save_image_zscan();
  void update_image_zscan(const QString&);

  void  update_autofocus_config(const double, const int);
  void acquire_autofocus_config();

  void mouseMoveEvent(QMouseEvent*);
  void mouseDoubleClickEvent(QMouseEvent*);

  void display_infoTab();

  void InfoMotionFinished();

  void makeProgBarVisible();
  void makeProgBarInvisible();
  void update_progBar(int);

 signals:

  void image_loaded(const cv::Mat&);

  void image_updated(const cv::Mat&);

  void image_zscan_updated(const cv::Mat&);
  void image_zscan_updated();

  void autofocus_config(const double, const int);

  void autofocus_request();

  void autofocus_emergencyStop_request();

  void sigRequestMoveRelative(double, double, double, double);

  void request_image();

  void cameraMotionIsFinished();
};

#endif // ASSEMBLYIMAGEVIEW_H

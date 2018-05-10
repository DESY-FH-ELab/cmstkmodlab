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

  QPushButton* autofocus_button() const { return AF_exe_button_; }

 protected:

  void keyReleaseEvent(QKeyEvent*);

  // image
  AssemblyUEyeView* img_ueye_;
  QScrollArea*      img_scroll_;

  QPushButton* img_load_button_;
  QPushButton* img_save_button_;
  // -------------------

  // auto-focusing
  AssemblyUEyeView* AF_ueye_;
  QScrollArea*      AF_scroll_;

  QLineEdit* AF_result_bestZ_lineed_;

  QLineEdit* AF_param_maxDZ_lineed_;
  QLineEdit* AF_param_Nstep_lineed_;

  QPushButton* AF_exe_button_;
  // -------------------

 public slots:

  void update_text(const double);

  void acquire_image_zscan(const QString&);

  void  update_autofocus_config(const double, const int);
  void acquire_autofocus_config();

 signals:

  void image_zscan_acquired(const cv::Mat&);

  void autofocus_config(const double, const int);

  void autofocus_request();
};

#endif // ASSEMBLYIMAGEVIEW_H

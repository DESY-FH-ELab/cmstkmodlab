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

#ifndef ASSEMBLYOBJECTFINDERPATRECVIEW_H
#define ASSEMBLYOBJECTFINDERPATRECVIEW_H

#include <AssemblyUEyeView.h>
#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyObjectFinderPatRecWidget.h>

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QMutex>

#include <opencv2/opencv.hpp>

class AssemblyObjectFinderPatRecView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyObjectFinderPatRecView(QWidget* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRecView() {}

  AssemblyObjectFinderPatRecWidget* PatRec_Widget() const { return patrec_wid_; }

  QPushButton* PatRec_exe_button() const { return patrec_exe_button_; }
  QPushButton* PatRec_exe_button_2() const { return patrec_exe_button_2_; }


  
  void connect_to_finder(const AssemblyObjectFinderPatRec* const);

 private:
  Q_DISABLE_COPY(AssemblyObjectFinderPatRecView)

  mutable QMutex mutex_;

 protected:

  void keyReleaseEvent(QKeyEvent *event);

  QScrollArea* scrollArea_1_;
  QScrollArea* scrollArea_2_;
  QScrollArea* scrollArea_3_;
  QScrollArea* scrollArea_4_;

  AssemblyUEyeView* imageView_1_;
  AssemblyUEyeView* imageView_2_;
  AssemblyUEyeView* imageView_3_;
  AssemblyUEyeView* imageView_4_;

  QPushButton* patrec_exe_button_;
  QLabel*      patrec_exe_label_;

  QPushButton* patrec_exe_button_2_;
  QLabel*      patrec_exe_label_2_;
  
  AssemblyObjectFinderPatRecWidget* patrec_wid_;
  AssemblyObjectFinderPatRecWidget* patrec_wid_2_;

  
  QLineEdit* patrec_res_dX_linee_;
  QLineEdit* patrec_res_dY_linee_;
  QLineEdit* patrec_res_ang_linee_;

  bool finder_connected_;

 public slots:

  void update_image_1(const QString&);
  void update_image_1(const cv::Mat&);

  void update_image_2(const QString&);
  void update_image_2(const cv::Mat&);

  void update_image_3(const QString&);
  void update_image_3(const cv::Mat&);

  void update_image_4(const QString&);
  void update_image_4(const cv::Mat&);

  void update_image(const int, const QString&);
  void update_image(const int, const cv::Mat&);

  void update_text(const double, const double, const double);

  void update_label(const int);

  void display_infoTab();
};

#endif // ASSEMBLYOBJECTFINDERPATRECVIEW_H

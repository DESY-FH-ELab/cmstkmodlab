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

#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeView.h>
#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyObjectFinderPatRecWidget.h>

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>

#include <opencv2/opencv.hpp>

class AssemblyObjectFinderPatRecView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyObjectFinderPatRecView(QWidget* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRecView() {}

  AssemblyObjectFinderPatRecWidget* PatRec_Widget() const { return w_patrec_; }

  void connect_to_finder(const AssemblyObjectFinderPatRec*);

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

  QRadioButton* radio1_;
  QRadioButton* radio2_;
  QRadioButton* radio3_;
  QRadioButton* radio4_;
  QRadioButton* radio5_;
  QRadioButton* radio6_;

  AssemblyObjectFinderPatRecWidget* w_patrec_;

  QLineEdit* patrec_res1_linee_;
  QLineEdit* patrec_res2_linee_;

  bool objfinder_connected_;

 public slots:

  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

  void updateText(const int, const double, const double, const double);

 signals:
};

#endif // ASSEMBLYOBJECTFINDERPATRECVIEW_H

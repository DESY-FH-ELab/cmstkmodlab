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

#ifndef ASSEMBLYOBJECTALIGNERVIEW_H
#define ASSEMBLYOBJECTALIGNERVIEW_H

#include <AssemblyUEyeView.h>
#include <AssemblyObjectAligner.h>
#include <AssemblyObjectFinderPatRecWidget.h>

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <opencv2/opencv.hpp>

class AssemblyObjectAlignerView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyObjectAlignerView(QWidget* parent=nullptr);
  virtual ~AssemblyObjectAlignerView() {}

  AssemblyObjectFinderPatRecWidget* PatRecOne_Widget() const { return w_patrecOne_; }
  AssemblyObjectFinderPatRecWidget* PatRecTwo_Widget() const { return w_patrecTwo_; }

//!!  void connect_to_finder(const AssemblyObjectFinderPatRec* const);

  AssemblyObjectAligner::Configuration get_configuration(bool&) const;

 protected:

  void keyReleaseEvent(QKeyEvent *event);

  QLabel*    alignm_objDX_label_;
  QLineEdit* alignm_objDX_linee_;

  QLabel*    alignm_objDY_label_;
  QLineEdit* alignm_objDY_linee_;

  QLabel*    alignm_angtgt_label_;
  QLineEdit* alignm_angtgt_linee_;

  QRadioButton* alignm_exemeas_radbu_;
  QPushButton*  alignm_exemeas_pusbu_;

  QRadioButton* alignm_exealig_radbu_;
  QPushButton*  alignm_exealig_pusbu_;

  AssemblyObjectFinderPatRecWidget* w_patrecOne_;
  AssemblyObjectFinderPatRecWidget* w_patrecTwo_;

  QLabel*    alignm_mesang_label_;
  QLineEdit* alignm_mesang_linee_;

  AssemblyUEyeView* patrecOne_image_;
  QScrollArea*      patrecOne_scroll_;

  AssemblyUEyeView* patrecTwo_image_;
  QScrollArea*      patrecTwo_scroll_;

  bool finder_connected_;

 public slots:

  void enable();

  void transmit_configuration();

  void show_measured_angle(const double);

  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

//!!  void updateText(const int, const double, const double, const double);

 signals:

  void configuration(AssemblyObjectAligner::Configuration);
};

#endif // ASSEMBLYOBJECTALIGNERVIEW_H

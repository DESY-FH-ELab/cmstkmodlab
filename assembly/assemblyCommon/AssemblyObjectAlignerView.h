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
#include <ApplicationConfig.h>

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include <opencv2/opencv.hpp>

class AssemblyObjectAlignerView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyObjectAlignerView(QWidget* parent=nullptr);
  virtual ~AssemblyObjectAlignerView() {}

  QWidget* Configuration_Widget() const { return alignm_cfg_wid_; }
  QWidget* Results_Widget()       const { return alignm_res_wid_; }

  AssemblyObjectFinderPatRecWidget* PatRecOne_Widget() const { return patrecOne_wid_; }
  AssemblyObjectFinderPatRecWidget* PatRecTwo_Widget() const { return patrecTwo_wid_; }

  AssemblyUEyeView* PatRecOne_Image() const { return patrecOne_image_; }
  AssemblyUEyeView* PatRecTwo_Image() const { return patrecTwo_image_; }

  QPushButton* button_alignerEmergencyStop() const { return button_alignerEmergencyStop_; }

  AssemblyObjectAligner::Configuration get_configuration(bool&) const;

 protected:

  void keyReleaseEvent(QKeyEvent *event);

  QWidget* alignm_cfg_wid_;
  QWidget* alignm_res_wid_;

  QRadioButton* alignm_PSS_radbu_;
  QLineEdit*    alignm_PSS_dX_linee_;
  QLineEdit*    alignm_PSS_dY_linee_;

  QRadioButton* alignm_PSP_radbu_;
  QLineEdit*    alignm_PSP_dX_linee_;
  QLineEdit*    alignm_PSP_dY_linee_;

  QCheckBox* alignm_angtgt_calc_checkbox_;
  QLineEdit* alignm_angtgt_linee_;

  QCheckBox* alignm_completeAtPosOne_checkbox_;
  QCheckBox* alignm_useAutoFocusing_checkbox_;

  QRadioButton* alignm_exemeas_radbu_;
  QPushButton*  alignm_exemeas_pusbu_;

  QRadioButton* alignm_exealig_radbu_;
  QPushButton*  alignm_exealig_pusbu_;

  QLineEdit* alignm_angmax_dontIter_linee_;
  QLineEdit* alignm_angmax_complete_linee_;

  AssemblyObjectFinderPatRecWidget* patrecOne_wid_;
  AssemblyObjectFinderPatRecWidget* patrecTwo_wid_;

  QLineEdit* alignm_mesang_linee_;

  AssemblyUEyeView* patrecOne_image_;
  QScrollArea*      patrecOne_scroll_;

  AssemblyUEyeView* patrecTwo_image_;
  QScrollArea*      patrecTwo_scroll_;

  QPushButton* button_alignerEmergencyStop_;

  bool finder_connected_;

  ApplicationConfig* config_;

 public slots:

  void enable();

  void transmit_configuration();

  void show_measured_angle(const double);

  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

  void update_target_angle(const bool);
  void update_target_angle(const int);

  void update_autofocusing_checkbox(const int);

  void display_infoTab();

  void set_alignmentMode_PSP();
  void set_alignmentMode_PSS();

  void update_templates(const bool);

 signals:

  void configuration(AssemblyObjectAligner::Configuration);
};

#endif // ASSEMBLYOBJECTALIGNERVIEW_H

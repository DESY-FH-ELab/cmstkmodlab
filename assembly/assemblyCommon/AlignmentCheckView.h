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

#ifndef ALIGNMENTCHECKVIEW_H
#define ALIGNMENTCHECKVIEW_H

#include <AssemblyUEyeView.h>
#include <AlignmentCheck.h>
#include <AssemblyObjectFinderPatRecWidget.h>
#include <AssemblyObjectFinderPatRecWidget.h>
#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include <opencv2/opencv.hpp>

class AlignmentCheckView : public QWidget
{
 Q_OBJECT

 public:

  explicit AlignmentCheckView(QWidget* parent=nullptr);
  virtual ~AlignmentCheckView() {}

  QWidget* Configuration_Widget() const { return alignm_cfg_wid_; }
  QWidget* Results_Widget()       const { return alignm_res_wid_; }

  
  AssemblyObjectFinderPatRecWidget* PatRecOne_Widget() const { return patrecOne_wid_; }
  AssemblyObjectFinderPatRecWidget* PatRecTwo_Widget() const { return patrecTwo_wid_; }
  AssemblyObjectFinderPatRecWidget* PatRecThree_Widget() const { return patrecOne_wid_; }
  AssemblyObjectFinderPatRecWidget* PatRecFour_Widget() const { return patrecTwo_wid_; }

  AssemblyUEyeView* PatRecOne_Image() const { return patrecOne_image_; }
  AssemblyUEyeView* PatRecTwo_Image() const { return patrecTwo_image_; }
  AssemblyUEyeView* PatRecThree_Image() const { return patrecThree_image_;}
  AssemblyUEyeView* PatRecFour_Image() const { return patrecFour_image_;}
  
  QPushButton* button_emergencyStop() const { return button_emergencyStop_; }

  AlignmentCheck::Configuration get_configuration(bool&) const;

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

  QLabel* alignm_init_label_;
  
  QRadioButton* alignm_exealig_radbu_;
  QPushButton*  alignm_exealig_pusbu_;

  QLineEdit* alignm_angmax_dontIter_linee_;
  QLineEdit* alignm_angmax_complete_linee_;

  AssemblyObjectFinderPatRecWidget* patrecOne_wid_;
  AssemblyObjectFinderPatRecWidget* patrecTwo_wid_;

  AssemblyObjectFinderPatRecWidget* patrecThree_wid_;
  AssemblyObjectFinderPatRecWidget* patrecFour_wid_;

  QLineEdit* alignm_mesang_linee_;


  QLineEdit* alignm_pspblx_linee_;
  QLineEdit* alignm_pssblx_linee_;
  QLineEdit* alignm_psstlx_linee_;
  QLineEdit* alignm_psptlx_linee_;
  QLineEdit* alignm_pspbly_linee_;
  QLineEdit* alignm_pssbly_linee_;
  QLineEdit* alignm_psstly_linee_;
  QLineEdit* alignm_psptly_linee_;

  QLineEdit* alignm_angle_linee_;
  QLineEdit* alignm_xoffs_linee_;
  QLineEdit* alignm_yoffs_linee_;
  
  AssemblyUEyeView* patrecOne_image_;
  QScrollArea*      patrecOne_scroll_;

  AssemblyUEyeView* patrecTwo_image_;
  QScrollArea*      patrecTwo_scroll_;

  AssemblyUEyeView* patrecThree_image_;
  QScrollArea*      patrecThree_scroll_;

  AssemblyUEyeView* patrecFour_image_;
  QScrollArea*      patrecFour_scroll_;

  QPushButton* button_emergencyStop_;

  bool finder_connected_;

 public slots:

  void enable();

  void transmit_configuration();

  void show_measured_angle(const double);

  void show_pspbl_pos(const double, const double);
  void show_pssbl_pos(const double, const double);
  void show_psptl_pos(const double, const double);
  void show_psstl_pos(const double, const double);

  void show_offs_pos(const double, const double);
  
  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

   void update_autofocusing_checkbox(const int);

 signals:

  void configuration(AlignmentCheck::Configuration);
};

#endif // ASSEMBLYOBJECTALIGNERVIEW_H

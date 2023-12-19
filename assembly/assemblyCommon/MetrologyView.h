/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2023 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef METROLOGYVIEW_H
#define METROLOGYVIEW_H

#include <AssemblyUEyeView.h>
#include <AssemblyObjectFinderPatRecWidget.h>
#include <ApplicationConfig.h>
#include <Metrology.h>

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include <opencv2/opencv.hpp>

class MetrologyView : public QWidget
{
 Q_OBJECT

 public:

  explicit MetrologyView(QWidget* parent=nullptr);
  virtual ~MetrologyView() {}

  QWidget* Configuration_Widget() const { return metro_cfg_wid_; }
  QWidget* Results_Widget()       const { return metro_res_wid_; }

  AssemblyObjectFinderPatRecWidget* PatRecOne_Widget() const { return patrecOne_wid_; }
  AssemblyObjectFinderPatRecWidget* PatRecTwo_Widget() const { return patrecTwo_wid_; }

  AssemblyUEyeView* PatRecOne_Image() const { return patrecOne_image_; }
  AssemblyUEyeView* PatRecTwo_Image() const { return patrecTwo_image_; }
  AssemblyUEyeView* PatRecThree_Image() const { return patrecThree_image_; }
  AssemblyUEyeView* PatRecFour_Image() const { return patrecFour_image_; }

  QPushButton* button_metrologyEmergencyStop() const { return button_metrologyEmergencyStop_; }

  Metrology::Configuration get_configuration(bool&) const;

 protected:

  QWidget* metro_cfg_wid_;
  QWidget* metro_res_wid_;

  QCheckBox* metro_completeAtPosOne_checkbox_;
  QCheckBox* metro_useAutoFocusing_checkbox_;

  QPushButton*  metro_exemetro_pusbu_;

  QLineEdit* alignm_angmax_dontIter_linee_;
  QLineEdit* alignm_angmax_complete_linee_;

  AssemblyObjectFinderPatRecWidget* patrecOne_wid_;
  AssemblyObjectFinderPatRecWidget* patrecTwo_wid_;
  AssemblyObjectFinderPatRecWidget* patrecThree_wid_;
  AssemblyObjectFinderPatRecWidget* patrecFour_wid_;

  QLineEdit* metro_mesang_PSp_linee_;
  QLineEdit* metro_mesang_PSs_linee_;
  QLineEdit* metro_dx_linee_;
  QLineEdit* metro_dy_linee_;
  QLineEdit* metro_da_linee_;

  AssemblyUEyeView* patrecOne_image_;
  QScrollArea*      patrecOne_scroll_;

  AssemblyUEyeView* patrecTwo_image_;
  QScrollArea*      patrecTwo_scroll_;

  AssemblyUEyeView* patrecThree_image_;
  QScrollArea*      patrecThree_scroll_;

  AssemblyUEyeView* patrecFour_image_;
  QScrollArea*      patrecFour_scroll_;

  QPushButton* button_metrologyEmergencyStop_;

  bool finder_connected_;

  ApplicationConfig* config_;

 public slots:

  void transmit_configuration();

  void show_measured_angle(const bool, const double);

  void show_results(const double, const double, const double);

  void display_infoTab();

  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

  void update_templates(const bool);

 signals:

  void configuration(Metrology::Configuration);

 };

#endif // METROLOGYVIEW_H

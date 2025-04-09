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

#ifndef ASSEMBLYOBJECTFINDERPATRECWIDGET_H
#define ASSEMBLYOBJECTFINDERPATRECWIDGET_H

#include <LStepExpressMotionManager.h>
#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeView.h>
#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyMultiPickupTester.h>

#include <string>

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QKeyEvent>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>

#include <opencv2/opencv.hpp>

class AssemblyObjectFinderPatRecWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyObjectFinderPatRecWidget(const bool=false, QWidget* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRecWidget() {}

  QVBoxLayout* layout() const { return layout_; }

  QPushButton* template_load_button()   const { return templa_load_button_; }
  QLineEdit*   template_file_lineEdit() const { return templa_file_linee_; }

  QLineEdit*    threshold_lineEdit() const { return thresh_thresh_linee_; }

  AssemblyObjectFinderPatRec::Configuration get_configuration(bool&) const;

 protected:

  QVBoxLayout* layout_;

  QPushButton* templa_load_button_;
  QLineEdit*   templa_file_linee_;

  QPushButton* suggest_psp1_button_;
  QPushButton* suggest_psp2_button_;
  QPushButton* suggest_pss1_button_;
  QPushButton* suggest_pss2_button_;

  QLineEdit*    thresh_thresh_linee_;

  ApplicationConfig* config_;

 public slots:

  void load_image_template();
  void load_image_template_from_path(const QString&);
  void load_image_template_from_config();

  void transmit_configuration();

 signals:

  void updated_image_template_path(const QString&);
  void updated_image_template(const cv::Mat&);

  void configuration(const AssemblyObjectFinderPatRec::Configuration&);

  void invalid_configuration() const;
};

#endif // ASSEMBLYOBJECTFINDERPATRECWIDGET_H

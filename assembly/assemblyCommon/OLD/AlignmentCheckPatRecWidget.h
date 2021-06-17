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

#ifndef ALIGNMENTCHECKPATRECWIDGET_H
#define ALIGNMENTCHECKPATRECWIDGET_H

// #include <ConradModel.h> // CONRAD
#include <VellemanModel.h> // VELLEMAN
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

class AlignmentCheckPatRecWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AlignmentCheckPatRecWidget(QWidget* parent=nullptr);
  virtual ~AlignmentCheckPatRecWidget() {}

  QVBoxLayout* layout() const { return layout_; }

  QPushButton* template_load_button()   const { return templa_load_button_; }
  QLineEdit*   template_file_lineEdit() const { return templa_file_linee_; }

  QRadioButton* threshold_button()   const { return thresh_thresh_radbu_; }
  QLineEdit*    threshold_lineEdit() const { return thresh_thresh_linee_; }

  QRadioButton* adaptiveThreshold_button()   const { return thresh_adathr_radbu_; }
  QLineEdit*    adaptiveThreshold_lineEdit() const { return thresh_adathr_linee_; }

  AlignmentCheck::Configuration get_configuration(bool&) const;

 protected:

  QVBoxLayout* layout_;

  QPushButton* templa_load_button_;
  QLineEdit*   templa_file_linee_;

  QRadioButton* thresh_thresh_radbu_;
  QLineEdit*    thresh_thresh_linee_;

  QRadioButton* thresh_adathr_radbu_;
  QLineEdit*    thresh_adathr_linee_;


 public slots:

  void load_image_template();
  void load_image_template_from_path(const QString&);

  void transmit_configuration();

 signals:

  void updated_image_template_path(const QString&);
  void updated_image_template(const cv::Mat&);

  void configuration(const AssemblyObjectFinderPatRec::Configuration&);
};

#endif // ASSEMBLYOBJECTFINDERPATRECWIDGET_H

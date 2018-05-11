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

#include <ConradModel.h>
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

  explicit AssemblyObjectFinderPatRecWidget(QWidget* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRecWidget() {}

  QVBoxLayout* layout() const { return layout_; }

 protected:

  QVBoxLayout* layout_;

  QPushButton* templa_load_button_;
  QLineEdit*   templa_file_lineed_;

  QRadioButton* thresh_thresh_radbu_;
  QLineEdit*    thresh_thresh_linee_;

  QRadioButton* thresh_adathr_radbu_;
  QLineEdit*    thresh_adathr_linee_;

  QLabel*    angles_prescan_label_;
  QLineEdit* angles_prescan_linee_;

  QLabel*    angles_finemax_label_;
  QLineEdit* angles_finemax_linee_;

  QLabel*    angles_finestep_label_;
  QLineEdit* angles_finestep_linee_;

  AssemblyObjectFinderPatRec::Configuration configuration_;

 public slots:

  void load_image_template();
  void load_image_template_from_path(const QString&);

  void update_configuration();

 signals:

  void updated_image_template_line(const QString&);
  void updated_image_template(const cv::Mat&);

  void configuration_updated(const AssemblyObjectFinderPatRec::Configuration&);
  void configuration_updated();
};

#endif // ASSEMBLYOBJECTFINDERPATRECWIDGET_H

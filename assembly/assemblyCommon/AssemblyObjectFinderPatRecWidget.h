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
#include <QFormLayout>
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

  explicit AssemblyObjectFinderPatRecWidget(const QString&, QWidget* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRecWidget() {}

  QFormLayout* layout() const { return layout_; }

  const AssemblyStringWidget* widget_angrough() { return sw_angrough_; }
  const AssemblyStringWidget* widget_angscanp() { return sw_angscanp_; }

 protected:

  QFormLayout* layout_;
  QPushButton* button_;
  QLineEdit*   lineed_;
  QLabel*      label_;
  QGroupBox*   groupBox1_;
  QGroupBox*   groupBox2_;

  QRadioButton* radio1_;
  QRadioButton* radio2_;
  QRadioButton* radio3_;
  QRadioButton* radio4_;
  QRadioButton* radio5_;
  QRadioButton* radio6_;

  QVBoxLayout* vbox1_;
  QVBoxLayout* vbox2_;

  AssemblyStringWidget* sw_angrough_;
  AssemblyStringWidget* sw_angscanp_;

 public slots:

  void execute();

  void change_label(const int);

 signals:

  void mode(int, int);

  void sendPosition(int, double, double, double);
};

#endif // ASSEMBLYOBJECTFINDERPATRECWIDGET_H

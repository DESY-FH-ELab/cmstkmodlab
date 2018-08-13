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

#ifndef ASSEMBLYMULTIPICKUPTESTERWIDGET_H
#define ASSEMBLYMULTIPICKUPTESTERWIDGET_H

#include <LStepExpressMotionManager.h>
#include <AssemblyMultiPickupTester.h>

#include <QWidget>
#include <QString>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <opencv2/opencv.hpp>

class AssemblyMultiPickupTesterWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyMultiPickupTesterWidget(const LStepExpressMotionManager* const, QWidget* parent=nullptr);
  virtual ~AssemblyMultiPickupTesterWidget() {}

  QGridLayout* layout() const { return layout_; }

 protected:

  const LStepExpressMotionManager* const motion_manager_;

  QGridLayout* layout_;

  QPushButton* exe_button_;

  QLabel *     pickup_label_;
  QLineEdit*   pickup_lineed_;
  QPushButton* pickup_button_;

  QLabel*      measur_label_;
  QLineEdit*   measur_lineed_;
  QPushButton* measur_button_;

  QLabel*      iteraN_label_;
  QLineEdit*   iteraN_lineed_;

 public slots:

  void execute();

  void update_position_measurement();
  void update_position_pickup();

  void enable(const bool);

 signals:

  void multipickup_request(const AssemblyMultiPickupTester::Configuration&);
};

#endif // ASSEMBLYMULTIPICKUPTESTERWIDGET_H

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

#ifndef ASSEMBLYSUBASSEMBLYPICKUPWIDGET_H
#define ASSEMBLYSUBASSEMBLYPICKUPWIDGET_H

#include <LStepExpressMotionManager.h>
#include <AssemblySubassemblyPickup.h>
#include <AssemblyAssemblyTextWidget.h>
#include <AssemblyAssemblyActionWidget.h>

#include <QWidget>
#include <QString>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <opencv2/opencv.hpp>

class AssemblySubassemblyPickupWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblySubassemblyPickupWidget(const QObject* const, QWidget* parent=nullptr);
  virtual ~AssemblySubassemblyPickupWidget() {}

  QGridLayout* layout() const { return layout_; }

 protected:

  QCheckBox* smartMove_checkbox_;

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



 signals:

  //void multipickup_request(const AssemblySubassemblyPickup::Configuration&);
};

#endif // ASSEMBLYSUBASSEMBLYPICKUPWIDGET_H

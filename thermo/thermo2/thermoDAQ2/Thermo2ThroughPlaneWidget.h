/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2THROUGHPLANEWIDGET_H
#define THERMO2THROUGHPLANEWIDGET_H

#include <QCheckBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QSvgWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDialog>
#include <QTextEdit>

#include "Thermo2ThroughPlaneModel.h"

class ThroughPlaneSVGWidget : public QSvgWidget
{
  Q_OBJECT
 public:

  ThroughPlaneSVGWidget(QWidget* parent = 0);
  virtual int heightForWidth(int) const;
  virtual int widthForHeight(int) const;
  
 protected:

  virtual void resizeEvent(QResizeEvent *event);

 signals:

 private:

};

class Thermo2ThroughPlaneWidget : public QWidget
{
  Q_OBJECT
 public:

  explicit Thermo2ThroughPlaneWidget(Thermo2ThroughPlaneModel* model,
		  QWidget *parent = 0);

 protected:

  Thermo2ThroughPlaneModel* model_;

  QCheckBox* mattermostStatus_;
  ThroughPlaneSVGWidget* svgWidget_;
  
 public slots:

  void mattermostChanged();
  void updateInfo();

 protected slots:

};

#endif // THERMO2THROUGHPLANEWIDGET_H

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <cmath>

#include <QBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QDialogButtonBox>

#include <nqlogger.h>

#include "ThermoDisplay2LineSeries.h"
#include "ThermoDisplay2ValueAxis.h"

ThermoDisplay2ValueAxis::ThermoDisplay2ValueAxis(int precision)
 : QValueAxis(),
   precision_(precision),
   axisMode_(AxisModeFull)
{

}

void ThermoDisplay2ValueAxis::refresh(QList<QAbstractSeries*> series)
{
  if (axisMode_!=AxisModeUser) {
    min_ = std::numeric_limits<qreal>::max();
    max_ = -std::numeric_limits<qreal>::max();

    bool hasValues = false;
    for (QAbstractSeries * s : series) {
      ThermoDisplay2LineSeries* ls = dynamic_cast<ThermoDisplay2LineSeries*>(s);

      bool hasAxis = false;
      auto axes = ls->attachedAxes();
      for (QAbstractAxis * axis : axes) {
        if (axis==this) {
          hasAxis = true;
          break;
        }
      }
      if (!hasAxis) continue;

      if (ls && ls->isInitialized()) {
        min_ = std::min(min_, ls->minY());
        max_ = std::max(max_, ls->maxY());
        hasValues = true;
      }
    }
    if (!hasValues) return;

    qreal deltaY = max_-min_;
    if (deltaY<5.0) deltaY = 5.0;

    setRange(std::pow(10,-precision_)*std::round(std::pow(10,precision_)*(min_-0.1*deltaY)),
             std::pow(10,-precision_)*std::round(std::pow(10,precision_)*(max_+0.1*deltaY)));
  } else {

    setRange(userMin_, userMax_);
  }
}

void ThermoDisplay2ValueAxis::configure()
{
  ThermoDisplay2ValueAxisDialog dialog(precision_);

  dialog.setAxisMode(axisMode_);
  dialog.setMinMaxRange(-std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max());
  if (axisMode_==AxisModeUser) {
    dialog.setUserRange(userMin_, userMax_);
  } else {
    qreal deltaY = max_-min_;
    if (deltaY<5.0) deltaY = 5.0;
    dialog.setUserRange(std::pow(10,-precision_)*std::round(std::pow(10,precision_)*(min_-0.1*deltaY)),
			std::pow(10,-precision_)*std::round(std::pow(10,precision_)*(max_+0.1*deltaY)));
  }
  
  NQLogDebug("ThermoDisplay2ValueAxis") << "min_ = " << min_ << "; max_ = " << max_;

  int ret = dialog.exec();

  if (ret==QDialog::Accepted) {
    axisMode_ = dialog.getAxisMode();
    if (axisMode_==AxisModeUser) {
      dialog.getUserRange(userMin_, userMax_);
    }
    emit axisModeChanged();
  }
}

ThermoDisplay2ValueAxisDialog::ThermoDisplay2ValueAxisDialog(int precision, QWidget* parent)
  : QDialog(parent),
    precision_(precision)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  QGroupBox *groupBox = new QGroupBox("Axis mode");

  buttonGroup_ = new QButtonGroup();
  buttonGroup_->setExclusive(true);

  modeFull_ = new QRadioButton("Auto range");
  buttonGroup_->addButton(modeFull_, ThermoDisplay2ValueAxis::AxisModeFull);
  modeUser_ = new QRadioButton("User range");
  buttonGroup_->addButton(modeUser_, ThermoDisplay2ValueAxis::AxisModeUser);

  connect(buttonGroup_, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(buttonClicked(QAbstractButton *)));

  layout->addWidget(groupBox);

  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(modeFull_);
  vbox->addWidget(modeUser_);
  vbox->addStretch(1);

  QFormLayout *flayout = new QFormLayout;

  userMin_ = new QDoubleSpinBox();
  userMin_->setDecimals(precision_);
  flayout->addRow("Minimum", userMin_);
  connect(userMin_, SIGNAL(valueChanged(double)),
          this, SLOT(userMinChanged(double)));

  userMax_ = new QDoubleSpinBox();
  userMax_->setDecimals(precision_);
  flayout->addRow("Maximum", userMax_);
  connect(userMax_, SIGNAL(valueChanged(double)),
          this, SLOT(userMaxChanged(double)));

  vbox->addLayout(flayout);

  groupBox->setLayout(vbox);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                     QDialogButtonBox::Cancel);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  layout->addWidget(buttonBox);
}

void ThermoDisplay2ValueAxisDialog::setAxisMode(ThermoDisplay2ValueAxis::AxisMode axisMode)
{
  axisMode_ = axisMode;

  if (axisMode_==ThermoDisplay2ValueAxis::AxisModeUser) {
    modeUser_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2ValueAxis::AxisModeFull) {
    modeFull_->setChecked(true);
  }

  if (axisMode_==ThermoDisplay2ValueAxis::AxisModeUser) {
    userMin_->setEnabled(true);
    userMax_->setEnabled(true);
  } else {
    userMin_->setEnabled(false);
    userMax_->setEnabled(false);
  }
}

void ThermoDisplay2ValueAxisDialog::setMinMaxRange(const qreal min,
                                                   const qreal max)
{
  absoluteMin_ = min;
  userMin_->setMinimum(-std::numeric_limits<qreal>::max());
  userMax_->setMinimum(-std::numeric_limits<qreal>::max());

  absoluteMax_ = max;
  userMin_->setMaximum(std::numeric_limits<qreal>::max());
  userMax_->setMaximum(std::numeric_limits<qreal>::max());
}

void ThermoDisplay2ValueAxisDialog::setUserRange(const qreal min,
                                                 const qreal max)
{
  qreal theMin = min;
  qreal theMax = max;
  if (theMin>theMax) {
    theMin = max;
    theMax = min;
  }

  userMin_->setValue(theMin);
  userMax_->setValue(theMax);

  userMin_->setMaximum(theMax);
  userMax_->setMinimum(theMin);
}

void ThermoDisplay2ValueAxisDialog::getUserRange(qreal& min,
                                                 qreal& max)
{
  min = userMin_->value();
  max = userMax_->value();
}

void ThermoDisplay2ValueAxisDialog::userMinChanged(double value)
{
  userMax_->setMinimum(value);
}

void ThermoDisplay2ValueAxisDialog::userMaxChanged(double value)
{
  userMin_->setMaximum(value);
}

void ThermoDisplay2ValueAxisDialog::buttonClicked(QAbstractButton *)
{
  int id = buttonGroup_->checkedId();
  
  if (id==ThermoDisplay2ValueAxis::AxisModeUser) {
    axisMode_ = ThermoDisplay2ValueAxis::AxisModeUser;
  } else {
    axisMode_ = ThermoDisplay2ValueAxis::AxisModeFull;
  }

  if (axisMode_==ThermoDisplay2ValueAxis::AxisModeUser) {
    userMin_->setEnabled(true);
    userMax_->setEnabled(true);
  } else {
    userMin_->setEnabled(false);
    userMax_->setEnabled(false);
  }
}

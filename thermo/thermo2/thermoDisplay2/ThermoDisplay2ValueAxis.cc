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

#include <QBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QDialogButtonBox>

#include <nqlogger.h>

#include "ThermoDisplay2LineSeries.h"
#include "ThermoDisplay2ValueAxis.h"

ThermoDisplay2ValueAxis::ThermoDisplay2ValueAxis()
 : QValueAxis(),
   axisMode_(AxisModeFull)
{

}

void ThermoDisplay2ValueAxis::refresh(QList<QAbstractSeries*> series)
{
  if (axisMode_!=AxisModeUser) {
    qreal minY = std::numeric_limits<qreal>::max();
    qreal maxY = -std::numeric_limits<qreal>::max();

    bool hasValues = false;
    for (QList<QAbstractSeries*>::Iterator it = series.begin();
        it!=series.end();
        ++it) {
      ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
      if (s && s->isInitialized()) {
        minY = std::min(minY, s->minY());
        maxY = std::max(maxY, s->maxY());
        hasValues = true;
      }
    }
    if (!hasValues) return;

    qreal deltaY = maxY-minY;
    if (deltaY<5.0) deltaY = 5.0;

    setRange(minY-0.1*deltaY, maxY+0.1*deltaY);

  } else {

    setRange(userMin_, userMax_);
  }
}

void ThermoDisplay2ValueAxis::configure()
{
	ThermoDisplay2ValueAxisDialog dialog;

	dialog.setAxisMode(axisMode_);
  dialog.setMinMaxRange(min_, max_);
  dialog.setUserRange(userMin_, userMax_);

  int ret = dialog.exec();

  if (ret==QDialog::Accepted) {
    axisMode_ = dialog.getAxisMode();
    if (axisMode_==AxisModeUser) {
      dialog.getUserRange(userMin_, userMax_);
    }
    emit axisModeChanged();
  }
}

ThermoDisplay2ValueAxisDialog::ThermoDisplay2ValueAxisDialog(QWidget* parent)
 : QDialog(parent)
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
  flayout->addRow("Minimum", userMin_);
  connect(userMin_, SIGNAL(valueChanged(double)),
          this, SLOT(userMinChanged(double)));

  userMax_ = new QDoubleSpinBox();
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
  userMin_->setMinimum(std::numeric_limits<qreal>::min());
  userMax_->setMinimum(std::numeric_limits<qreal>::min());

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

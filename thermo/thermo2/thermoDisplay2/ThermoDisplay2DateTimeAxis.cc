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
#include "ThermoDisplay2DateTimeAxis.h"

ThermoDisplay2DateTimeAxis::ThermoDisplay2DateTimeAxis()
 : QDateTimeAxis(),
   axisMode_(AxisMode15Minutes)
{

}

void ThermoDisplay2DateTimeAxis::refresh(QList<QAbstractSeries*> series)
{
  QDateTime dtMin, dtMax;
  int temp;

  if (axisMode_!=AxisModeUser) {

    qreal minX = std::numeric_limits<qreal>::max();
    qreal maxX = -std::numeric_limits<qreal>::max();

    bool hasValues = false;
    for (QAbstractSeries * s : series) {
      ThermoDisplay2LineSeries* ls = dynamic_cast<ThermoDisplay2LineSeries*>(s);
      if (ls && ls->isInitialized()) {
        minX = std::min(minX, ls->minX());
        maxX = std::max(maxX, ls->maxX());
        hasValues = true;
      }
    }
    if (!hasValues) return;

    min_ = QDateTime::fromMSecsSinceEpoch(minX);
    max_ = QDateTime::fromMSecsSinceEpoch(maxX);

    dtMin = min_;
    dtMax = max_;
   
    QDateTime dtTemp = dtMax;
    if (axisMode_==AxisMode15Minutes) {
      dtTemp = dtTemp.addSecs(-15 * 60);
    } else if (axisMode_==AxisMode30Minutes) {
      dtTemp = dtTemp.addSecs(-30 * 60);
    } else if (axisMode_==AxisMode1Hour) {
      dtTemp = dtTemp.addSecs(-1 * 60 * 60);
    } else if (axisMode_==AxisMode2Hour) {
      dtTemp = dtTemp.addSecs(-2 * 60 * 60);
    } else if (axisMode_==AxisMode6Hour) {
      dtTemp = dtTemp.addSecs(-6 * 60 * 60);
    } else if (axisMode_==AxisMode12Hour) {
      dtTemp = dtTemp.addSecs(-12 * 60 * 60);
    } else if (axisMode_==AxisMode24Hour) {
      dtTemp = dtTemp.addSecs(-24 * 60 * 60);
    }

    if (axisMode_!=AxisModeFull) {
      dtMin = dtTemp;
    }
    
    qint64 deltaX = 0.01 * dtMin.secsTo(dtMax);
    dtMin = dtMin.addSecs(-deltaX);
    temp = dtMin.time().second();
    dtMin = dtMin.addSecs(-temp);
    
    dtMax = dtMax.addSecs(deltaX);
    temp = dtMax.time().second();
    dtMax = dtMax.addSecs(60-temp);
    
  } else {
    dtMin = userMin_;
    dtMax = userMax_;
  }

  setRange(dtMin, dtMax);
}

void ThermoDisplay2DateTimeAxis::configure()
{
  ThermoDisplay2DateTimeAxisDialog dialog;

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

ThermoDisplay2DateTimeAxisDialog::ThermoDisplay2DateTimeAxisDialog(QWidget* parent)
 : QDialog(parent)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  QGroupBox *groupBox = new QGroupBox("Axis mode");

  buttonGroup_ = new QButtonGroup();
  buttonGroup_->setExclusive(true);

  modeFull_ = new QRadioButton("Auto range");
  buttonGroup_->addButton(modeFull_, ThermoDisplay2DateTimeAxis::AxisModeFull);
  mode15Minutes_ = new QRadioButton("Last 15 minutes");
  buttonGroup_->addButton(mode15Minutes_, ThermoDisplay2DateTimeAxis::AxisMode15Minutes);
  mode30Minutes_ = new QRadioButton("Last 30 minutes");
  buttonGroup_->addButton(mode30Minutes_, ThermoDisplay2DateTimeAxis::AxisMode30Minutes);
  mode1Hour_ = new QRadioButton("Last hour");
  buttonGroup_->addButton(mode1Hour_, ThermoDisplay2DateTimeAxis::AxisMode1Hour);
  mode2Hour_ = new QRadioButton("Last 2 hours");
  buttonGroup_->addButton(mode2Hour_, ThermoDisplay2DateTimeAxis::AxisMode2Hour);
  mode6Hour_ = new QRadioButton("Last 6 hours");
  buttonGroup_->addButton(mode6Hour_, ThermoDisplay2DateTimeAxis::AxisMode6Hour);
  mode12Hour_ = new QRadioButton("Last 12 hours");
  buttonGroup_->addButton(mode12Hour_, ThermoDisplay2DateTimeAxis::AxisMode12Hour);
  mode24Hour_ = new QRadioButton("Last 24 hours");
  buttonGroup_->addButton(mode24Hour_, ThermoDisplay2DateTimeAxis::AxisMode24Hour);
  modeUser_ = new QRadioButton("User range");
  buttonGroup_->addButton(modeUser_, ThermoDisplay2DateTimeAxis::AxisModeUser);

  connect(buttonGroup_, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(buttonClicked(QAbstractButton *)));

  layout->addWidget(groupBox);

  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(modeFull_);
  vbox->addWidget(mode15Minutes_);
  vbox->addWidget(mode30Minutes_);
  vbox->addWidget(mode1Hour_);
  vbox->addWidget(mode2Hour_);
  vbox->addWidget(mode6Hour_);
  vbox->addWidget(mode12Hour_);
  vbox->addWidget(mode24Hour_);
  vbox->addWidget(modeUser_);
  vbox->addStretch(1);

  QFormLayout *flayout = new QFormLayout;

  userMin_ = new QDateTimeEdit();
  userMin_->setDisplayFormat("dd.MM.yyyy hh:mm");
  flayout->addRow("Start", userMin_);

  userMax_ = new QDateTimeEdit();
  userMax_->setDisplayFormat("dd.MM.yyyy hh:mm");
  flayout->addRow("End", userMax_);

  vbox->addLayout(flayout);

  groupBox->setLayout(vbox);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                     QDialogButtonBox::Cancel);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  layout->addWidget(buttonBox);
}

void ThermoDisplay2DateTimeAxisDialog::setAxisMode(ThermoDisplay2DateTimeAxis::AxisMode axisMode)
{
  axisMode_ = axisMode;

  if (axisMode_==ThermoDisplay2DateTimeAxis::AxisMode15Minutes) {
    mode15Minutes_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisMode30Minutes) {
    mode30Minutes_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisMode1Hour) {
    mode1Hour_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisMode2Hour) {
    mode2Hour_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisMode6Hour) {
    mode6Hour_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisMode12Hour) {
    mode12Hour_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisMode24Hour) {
    mode24Hour_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisModeUser) {
    modeUser_->setChecked(true);
  } else if (axisMode_==ThermoDisplay2DateTimeAxis::AxisModeFull) {
    modeFull_->setChecked(true);
  }

  if (axisMode_==ThermoDisplay2DateTimeAxis::AxisModeUser) {
    userMin_->setEnabled(true);
    userMax_->setEnabled(true);
  } else {
    userMin_->setEnabled(false);
    userMax_->setEnabled(false);
  }
}

void ThermoDisplay2DateTimeAxisDialog::setMinMaxRange(const QDateTime& dtMin,
                                                      const QDateTime& dtMax)
{
  userMin_->setMinimumDateTime(dtMin);
  userMax_->setMinimumDateTime(dtMin);

  userMin_->setMaximumDateTime(dtMax);
  userMax_->setMaximumDateTime(dtMax);
}

void ThermoDisplay2DateTimeAxisDialog::setUserRange(const QDateTime& dtMin,
                                                    const QDateTime& dtMax)
{
  userMin_->setDateTime(dtMin);
  userMax_->setDateTime(dtMax);
}

void ThermoDisplay2DateTimeAxisDialog::getUserRange(QDateTime& dtMin,
                                                    QDateTime& dtMax)
{
  dtMin = userMin_->dateTime();
  dtMax = userMax_->dateTime();
}

void ThermoDisplay2DateTimeAxisDialog::buttonClicked(QAbstractButton *)
{
  int id = buttonGroup_->checkedId();

  axisMode_ = static_cast<ThermoDisplay2DateTimeAxis::AxisMode>(id);

  if (axisMode_==ThermoDisplay2DateTimeAxis::AxisModeUser) {
    userMin_->setEnabled(true);
    userMax_->setEnabled(true);
  } else {
    userMin_->setEnabled(false);
    userMax_->setEnabled(false);
  }
}

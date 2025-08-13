/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "LeyboldGraphixOneWidget.h"

/**
  \brief Creates a new panel with all the controls and read-outs for the LeyboldGraphixOne
  vacuum gauge.
  */
LeyboldGraphixOneWidget::LeyboldGraphixOneWidget(LeyboldGraphixOneModel* model, QWidget *parent) :
    QWidget(parent), model_(model)
{
  // Create all the nescessary widgets
  leyboldCheckBox_ = new QCheckBox("Enable vacuum gauges", this);

  operationPanel_ = new QWidget(this);

  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  layout->addWidget(leyboldCheckBox_);
  layout->addWidget(operationPanel_);

  QVBoxLayout* operationLayout = new QVBoxLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);

  operationLayout->addWidget(new LeyboldGraphixOneGaugeWidget(model_, 1, this));

  // Connect all the signals
  connect(model_,
          SIGNAL(deviceStateChanged(State)),
          this,
          SLOT(updateDeviceState(State)));

  connect(model_,
          SIGNAL(controlStateChanged(bool)),
          this,
          SLOT(controlStateChanged(bool)));

  connect(model_,
          SIGNAL(informationChanged()),
          this,
          SLOT(updateInfo()));

  connect(leyboldCheckBox_,
          SIGNAL(toggled(bool)),
          model,
          SLOT(setDeviceEnabled(bool)));

  // Set GUI according to the current chiller state
  updateDeviceState( model_->getDeviceState() );
  updateInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
  */
void LeyboldGraphixOneWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);
  leyboldCheckBox_->setChecked( ready );
  operationPanel_->setEnabled( ready );
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void LeyboldGraphixOneWidget::controlStateChanged(bool enabled)
{
  leyboldCheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
  */
void LeyboldGraphixOneWidget::updateInfo()
{

}

/* GaugeWidget implementation */

const unsigned int LeyboldGraphixOneGaugeWidget::LCD_SIZE = 20;
const QString LeyboldGraphixOneGaugeWidget::LABEL_FORMAT = QString("gauge %1");

LeyboldGraphixOneGaugeWidget::LeyboldGraphixOneGaugeWidget(LeyboldGraphixOneModel* model,
                                                           unsigned int sensor,
                                                           QWidget *parent)
 : QGroupBox(parent),
   model_(model),
   sensor_(sensor)
{
  setTitle(LABEL_FORMAT.arg(sensor_));

  QFormLayout* layout = new QFormLayout(this);
  setLayout(layout);

  statusDisplay_ = new QLabel(VLeyboldGraphixOneTPG262::GetGaugeStatusAsString(VLeyboldGraphixOneTPG262::tpg262GaugeInvalidStatus).c_str(),
                              this);
  layout->addRow(statusDisplay_);

  pressureDisplay_ = new QLCDNumber(LCD_SIZE, this);
  pressureDisplay_->setSegmentStyle(QLCDNumber::Flat);
  pressureDisplay_->setSmallDecimalPoint(true);
  pressureDisplay_->setDigitCount(10);

  layout->addRow("p [mbar]", pressureDisplay_);

  connect(model_,
          SIGNAL(deviceStateChanged(State)),
          this,
          SLOT(updateDeviceState(State)));

  connect(model_,
          SIGNAL(controlStateChanged(bool)),
          this,
          SLOT(controlStateChanged(bool)));

  connect(model_,
          SIGNAL(informationChanged()),
          this,
          SLOT(updateInfo()));

  // Set GUI according to the current chiller state
  updateDeviceState( model_->getDeviceState() );
  updateInfo();
}

/// Updates the GUI according to the current device state.
void LeyboldGraphixOneGaugeWidget::updateDeviceState(State /*state*/)
{

}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void LeyboldGraphixOneGaugeWidget::controlStateChanged(bool enabled)
{
  if (enabled) {
    //State state = model_->getDeviceState();
  } else {

  }
}

/// Relays the new temperature to the history model.
void LeyboldGraphixOneGaugeWidget::updateInfo()
{
    char dummy[20];
    VLeyboldGraphixOneTPG262::GaugeStatus status;
    float pressure;

    if (sensor_==1) {
        status = model_->getStatus1();
        pressure = model_->getPressure1();
    } else {
        status = model_->getStatus2();
        pressure = model_->getPressure2();
    }

    statusDisplay_->setText(VLeyboldGraphixOneTPG262::GetGaugeStatusAsString(status).c_str());

    if (pressure<1) {
        sprintf(dummy, "%.02E", pressure);
    } else if (pressure<10) {
        sprintf(dummy, "%.01f", pressure);
    } else {
        sprintf(dummy, "%d", (int)pressure);
    }
    pressureDisplay_->display(dummy);
}

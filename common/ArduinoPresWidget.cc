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

#include "ArduinoPresWidget.h"

/**
  \brief Creates a new panel with all the controls and read-outs for the Arduino.
  */
ArduinoPresWidget::ArduinoPresWidget(ArduinoPresModel* model, QWidget *parent) :
    QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  arduinoCheckBox_ = new QCheckBox("Enable sensors", this);
  layout->addWidget(arduinoCheckBox_);

  operationPanel_ = new QWidget(this);
  QFormLayout* operationLayout = new QFormLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);
  layout->addWidget(operationPanel_);

  PressureALCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  PressureALCD_->setSegmentStyle(QLCDNumber::Flat);
  PressureALCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("pressure on sensor A (bar)"),
                             PressureALCD_);
                          
  PressureBLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  PressureBLCD_->setSegmentStyle(QLCDNumber::Flat);
  PressureBLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("pressure on sensor B (bar)"),
                             PressureBLCD_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(arduinoCheckBox_, SIGNAL(toggled(bool)),
          model, SLOT(setDeviceEnabled(bool)));

  // Set GUI according to the current sensor state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the pump.
 */
void ArduinoPresWidget::updateDeviceState(State newState) {

  bool ready = (newState == READY);
  arduinoCheckBox_->setChecked( ready );
  operationPanel_->setEnabled( ready );
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void ArduinoPresWidget::controlStateChanged(bool enabled) {
  
  arduinoCheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void ArduinoPresWidget::updateInfo() {

  char bufferPA[10];
  sprintf(bufferPA, "%.02f", model_->getPressureA());
  PressureALCD_->display(bufferPA);

  char bufferPB[10];
  sprintf(bufferPB, "%.02f", model_->getPressureB());
  PressureBLCD_->display(bufferPB);

}

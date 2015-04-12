#include "CoriFlowWidget.h"

/**
  \brief Creates a new panel with all the controls and read-outs for the cori flow
  */
CoriFlowWidget::CoriFlowWidget(CoriFlowModel* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  CoriFlowCheckBox_ = new QCheckBox("Enable flow meter", this);
  layout->addWidget(CoriFlowCheckBox_);

  operationPanel_ = new QWidget(this);
  QFormLayout* operationLayout = new QFormLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);
  layout->addWidget(operationPanel_);

  TempLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  TempLCD_->setSegmentStyle(QLCDNumber::Flat);
  TempLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Temperature (C)"),
                          TempLCD_);

  PresLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  PresLCD_->setSegmentStyle(QLCDNumber::Flat);
  PresLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Pressure (mBar)"),
                          PresLCD_);

  MeasureLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  MeasureLCD_->setSegmentStyle(QLCDNumber::Flat);
  MeasureLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Flow (g/h)"),
                          MeasureLCD_);


                          
  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(CoriFlowCheckBox_, SIGNAL(toggled(bool)),
          model, SLOT(setDeviceEnabled(bool)));


  // Set GUI according to the current pump state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the cori.
 */
void CoriFlowWidget::updateDeviceState(State newState) {

  bool ready = (newState == READY);
  CoriFlowCheckBox_->setChecked( ready );
  operationPanel_->setEnabled( ready );
}

/// Updates the GUI when the cori is enabled/disabled.
void CoriFlowWidget::controlStateChanged(bool enabled) {

  CoriFlowCheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}


/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void CoriFlowWidget::updateInfo() {

  char bufferP[10];
  sprintf(bufferP, "%.02f", model_->getTemp());
  TempLCD_->display(bufferP);

  sprintf(bufferP, "%.02f", model_->getPres());
  PresLCD_->display(bufferP);

  sprintf(bufferP, "%.02f", model_->getMeasure());
  MeasureLCD_->display(bufferP);

}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "KeithleyDAQ6510Widget.h"

KeithleyDAQ6510UpdateIntervalBox::KeithleyDAQ6510UpdateIntervalBox(KeithleyDAQ6510Model* model,
                                                                   QWidget *parent)
: QComboBox(parent),
  model_(model)
{
#ifdef USE_FAKEIO
  addItem("5 seconds", 5);
#endif
  addItem("10 seconds", 10);
  addItem("20 seconds", 20);
  addItem("30 seconds", 30);
  addItem("45 seconds", 45);
  addItem("1 minute", 60*1);
  addItem("2 minutes", 60*2);
  addItem("3 minutes", 60*3);
  addItem("4 minutes", 60*4);
  addItem("5 minutes", 60*5);

  int idx = findData(model_->getUpdateInterval());
  if (idx!=-1) setCurrentIndex(idx);

  connect(this,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(currentItemChanged(int)));

  connect(this,
          SIGNAL(valueChanged(int)),
          model_,
          SLOT(setUpdateInterval(int)));
}

void KeithleyDAQ6510UpdateIntervalBox::currentItemChanged(int idx)
{
  emit(valueChanged(itemData(idx).toInt()));
}

KeithleyDAQ6510Widget::KeithleyDAQ6510Widget(KeithleyDAQ6510Model* model,
                                             QWidget *parent)
 : QWidget(parent),
   model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  keithleyCheckBox_ = new QCheckBox("Enable Multimeter", this);
  layout->addWidget(keithleyCheckBox_);

  updateIntervalBox_ = new KeithleyDAQ6510UpdateIntervalBox(model_, this);
  layout->addWidget(updateIntervalBox_);

  sensorControlWidget_= new QWidget(this);
  layout->addWidget(sensorControlWidget_);

  QGridLayout* sensorLayout = new QGridLayout(sensorControlWidget_);
  sensorControlWidget_->setLayout(sensorLayout);

  // Add all the temperature sensor displays
  unsigned int i = 0;
  for (unsigned int card=0;card<2;++card) {
    for (unsigned int channel=0;channel<10;++channel) {
      unsigned int sensor = 100 * (card+1) + channel + 1;
      sensorLayout->addWidget(new KeithleyDAQ6510TemperatureWidget(model_, sensor, this),
                              i/5, i%5);
      i++;
    }
  }

  connect(keithleyCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(keithleyStateChanged(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  keithleyStateChanged(model_->getDeviceState());
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void KeithleyDAQ6510Widget::keithleyStateChanged(State newState)
{
  keithleyCheckBox_->setChecked(newState == READY || newState == INITIALIZING);
  updateIntervalBox_->setEnabled(newState == READY);
  sensorControlWidget_->setEnabled(newState == READY);
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void KeithleyDAQ6510Widget::controlStateChanged(bool enabled)
{
  if (enabled) {
    keithleyCheckBox_->setEnabled(true);
    keithleyStateChanged(model_->getDeviceState());
  } else {
    keithleyCheckBox_->setEnabled(false);
    updateIntervalBox_->setEnabled(false);
    sensorControlWidget_->setEnabled(false);
  }
}

KeithleyDAQ6510SensorModeWidget::KeithleyDAQ6510SensorModeWidget(KeithleyDAQ6510Model* model,
		unsigned int sensor,
		QWidget *parent)
 : QComboBox(parent),
	 model_(model),
	 sensor_(sensor)
{
  // No user editable text
  setEditable(false);

  for (const auto& kv : model_->getSensorModeNames()) {
    addItem(kv.second.c_str(), QVariant(kv.first));
  }

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(keithleyStateChanged(State)));

  connect(model_, SIGNAL(sensorStateChanged(uint,State)),
          this, SLOT(sensorStateChanged(uint,State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/// Convenience method to set all the widgets correctly.
void KeithleyDAQ6510SensorModeWidget::updateWidgets()
{
  State sensorState = model_->getSensorState(sensor_);

  setEnabled(sensorState == READY || sensorState == INITIALIZING);
}

/// Updates the GUI according to the current device state.
void KeithleyDAQ6510SensorModeWidget::keithleyStateChanged(State newState)
{
  setEnabled(newState == READY || newState == INITIALIZING);

  updateWidgets();
}

void KeithleyDAQ6510SensorModeWidget::indexChanged(int index)
{
  // NQLog("KeithleyDAQ6510SensorModeWidget", NQLog::Debug) << "indexChanged()";

  int userValue = itemData(index).toInt();

  if (model_->getSensorMode(sensor_)!=userValue) {
    model_->setSensorMode(sensor_, static_cast<VKeithleyDAQ6510::ChannelMode_t>(userValue));
  }
}

void KeithleyDAQ6510SensorModeWidget::updateDeviceState( State newState )
{
	bool ready = (newState == READY);

	controlStateChanged(ready);
}

void KeithleyDAQ6510SensorModeWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
}

/// Updates the GUI according to the current sensor state.
void KeithleyDAQ6510SensorModeWidget::sensorStateChanged(unsigned int sensor,
                                                          State /* state */)
{
  if ( sensor_ == sensor )
      updateWidgets();
}

void KeithleyDAQ6510SensorModeWidget::updateInfo()
{
  // NQLog("KeithleyDAQ6510SensorModeWidget", NQLog::Debug) << "updateInfo()";

  int mode = model_->getSensorMode(sensor_);
  int index = findData(mode);
  if (index!=currentIndex()) {
    setCurrentIndex(index);
  }
}

const unsigned int KeithleyDAQ6510TemperatureWidget::LCD_SIZE = 5;
const QString KeithleyDAQ6510TemperatureWidget::LABEL_FORMAT = QString("Sensor %1");

KeithleyDAQ6510TemperatureWidget::KeithleyDAQ6510TemperatureWidget(KeithleyDAQ6510Model* model,
                                                                   unsigned int sensor,
                                                                   QWidget *parent)
 : QWidget(parent),
   model_(model),
   sensor_(sensor)
{
  layout_ = new QFormLayout(this);
  setLayout(layout_);

  enabledCheckBox_ = new QCheckBox(LABEL_FORMAT.arg(sensor), this);

  sensorMode_ = new KeithleyDAQ6510SensorModeWidget(model_, sensor_, this);

  currentTempDisplay_ = new QLCDNumber(LCD_SIZE, this);
  currentTempDisplay_->setSegmentStyle(QLCDNumber::Flat);
  currentTempDisplay_->setSmallDecimalPoint(true);
  currentTempDisplay_->setDigitCount(5);

  connect(model_, SIGNAL(sensorStateChanged(uint,State)),
          this, SLOT(sensorStateChanged(uint,State)));

  connect(model_, SIGNAL(temperatureChanged(uint,double)),
          this, SLOT(sensorTemperatureChanged(uint,double)));

  connect(enabledCheckBox_, SIGNAL(toggled(bool)),
          this, SLOT(enabledCheckBoxToggled(bool)));

  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(keithleyStateChanged(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  layout_->addRow(enabledCheckBox_);
  currentTempLabel_ = new QLabel(QString::fromUtf8("T"));
  layout_->addRow(currentTempLabel_, currentTempDisplay_);

  //layout->addRow(QString::fromUtf8("T (°C)"), currentTempDisplay_);
  //layout->addRow(QString::fromUtf8("dT/dt (°C/min)") ,currentGradientDisplay_);

  updateWidgets();
}

/// Convenience method to set all the widgets correctly.
void KeithleyDAQ6510TemperatureWidget::updateWidgets()
{
  State sensorState = model_->getSensorState(sensor_);

  enabledCheckBox_->setChecked(sensorState == READY || sensorState == INITIALIZING);

  if (sensorState == READY) {
    currentTempLabel_->setEnabled( true );
    currentTempDisplay_->setEnabled( true );
  } else {
    currentTempLabel_->setEnabled( false );
    currentTempDisplay_->setEnabled( false );
  }
}

/// Updates the GUI according to the current device state.
void KeithleyDAQ6510TemperatureWidget::keithleyStateChanged(State newState)
{
  enabledCheckBox_->setEnabled(newState == READY || newState == INITIALIZING);

  updateWidgets();
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void KeithleyDAQ6510TemperatureWidget::controlStateChanged(bool enabled)
{
  if (enabled) {
    State state = model_->getDeviceState();
    enabledCheckBox_->setEnabled(state == READY || state == INITIALIZING);
    updateWidgets();
  } else {
    enabledCheckBox_->setEnabled(false);
    currentTempLabel_->setEnabled( false );
    currentTempDisplay_->setEnabled(false);
  }
}

/// Updates the GUI according to the current sensor state.
void KeithleyDAQ6510TemperatureWidget::sensorStateChanged(unsigned int sensor,
                                                          State /* state */)
{
  if ( sensor_ == sensor )
      updateWidgets();
}

/// Relays the new temperature to the history model.
void KeithleyDAQ6510TemperatureWidget::sensorTemperatureChanged(unsigned int sensor,
                                                                double temperature)
{
    if (sensor_!=sensor) return;
    currentTempDisplay_->display(QString::number(temperature, 'f', 2));
}

/// Updates the model according to the GUI change.
void KeithleyDAQ6510TemperatureWidget::enabledCheckBoxToggled(bool enabled)
{
  model_->setSensorEnabled(sensor_, enabled);
}

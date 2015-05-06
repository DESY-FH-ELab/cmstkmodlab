#include "KeithleyWidget.h"

KeithleyUpdateIntervalBox::KeithleyUpdateIntervalBox(KeithleyModel* model,
                                                     QWidget *parent) :
    QComboBox(parent),
    model_(model)
{
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

void KeithleyUpdateIntervalBox::currentItemChanged(int idx)
{
    emit(valueChanged(itemData(idx).toInt()));
}

KeithleyWidget::KeithleyWidget(KeithleyModel* model,
                                       QWidget *parent) :
    QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  keithleyCheckBox_ = new QCheckBox("Enable Multimeter", this);
  layout->addWidget(keithleyCheckBox_);

  updateIntervalBox_ = new KeithleyUpdateIntervalBox(model_, this);
  layout->addWidget(updateIntervalBox_);

  sensorControlWidget_= new QWidget(this);
  layout->addWidget(sensorControlWidget_);

  QGridLayout* sensorLayout = new QGridLayout(sensorControlWidget_);
  sensorControlWidget_->setLayout(sensorLayout);

  // Add all the temperature sensor displays
  for ( unsigned int i = 0; i < KeithleyModel::SENSOR_COUNT; ++i ) {
    sensorLayout->addWidget(new KeithleyTemperatureWidget(model_, i, this),
                            i/4, i%4);
  }

  connect(keithleyCheckBox_,
          SIGNAL(toggled(bool)),
          model_,
          SLOT(setDeviceEnabled(bool)));

  connect(model_,
          SIGNAL(deviceStateChanged(State)),
          this,
          SLOT(keithleyStateChanged(State)));

  connect(model_,
          SIGNAL(controlStateChanged(bool)),
          this,
          SLOT(controlStateChanged(bool)));

  keithleyStateChanged(model_->getDeviceState());
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void KeithleyWidget::keithleyStateChanged(State newState) {

  keithleyCheckBox_->setChecked(newState == READY || newState == INITIALIZING);
  updateIntervalBox_->setEnabled(newState == READY);
  sensorControlWidget_->setEnabled(newState == READY);
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void KeithleyWidget::controlStateChanged(bool enabled) {

  if (enabled) {
    keithleyCheckBox_->setEnabled(true);
    keithleyStateChanged(model_->getDeviceState());
  } else {
    keithleyCheckBox_->setEnabled(false);
    updateIntervalBox_->setEnabled(false);
    sensorControlWidget_->setEnabled(false);
  }
}

/* TemperatureWidget implementation */

const unsigned int KeithleyTemperatureWidget::LCD_SIZE = 5;
const QString KeithleyTemperatureWidget::LABEL_FORMAT = QString("Sensor %1");

KeithleyTemperatureWidget::KeithleyTemperatureWidget(KeithleyModel* model,
						     unsigned int sensor,
						     QWidget *parent) :
    QWidget(parent),
    model_(model),
    sensor_(sensor)
{
  layout_ = new QFormLayout(this);
  setLayout(layout_);

  enabledCheckBox_ = new QCheckBox(LABEL_FORMAT.arg(sensor), this);

  currentTempDisplay_ = new QLCDNumber(LCD_SIZE, this);
  currentTempDisplay_->setSegmentStyle(QLCDNumber::Flat);
  currentTempDisplay_->setSmallDecimalPoint(true);
  currentTempDisplay_->setDigitCount(5);

  currentGradientDisplay_ = new QLCDNumber(LCD_SIZE/2, this);
  currentGradientDisplay_->setSegmentStyle(QLCDNumber::Flat);
  currentGradientDisplay_->setSmallDecimalPoint(true);
  currentGradientDisplay_->setDigitCount(8);

  connect(model_,
          SIGNAL(sensorStateChanged(uint,State)),
          this,
          SLOT(sensorStateChanged(uint,State)));

  connect(model_,
          SIGNAL(temperatureChanged(uint,double)),
          this,
          SLOT(sensorTemperatureChanged(uint,double)));

  connect(model_,
          SIGNAL(temperatureGradientChanged(uint,double)),
          this,
          SLOT(sensorTemperatureGradientChanged(uint,double)));

  connect(enabledCheckBox_,
          SIGNAL(toggled(bool)),
          this,
          SLOT(enabledCheckBoxToggled(bool)));

  connect(model_,
          SIGNAL(deviceStateChanged(State)),
          this,
          SLOT(keithleyStateChanged(State)));

  connect(model_,
          SIGNAL(controlStateChanged(bool)),
          this,
          SLOT(controlStateChanged(bool)));

  layout_->addRow(enabledCheckBox_);
  currentTempLabel_ = new QLabel(QString::fromUtf8("T"));
  layout_->addRow(currentTempLabel_, currentTempDisplay_);
  currentGradientLabel_ = new QLabel(QString::fromUtf8("dT/dt"));
  layout_->addRow(currentGradientLabel_, currentGradientDisplay_);
  //layout->addRow(QString::fromUtf8("T (°C)"), currentTempDisplay_);
  //layout->addRow(QString::fromUtf8("dT/dt (°C/min)") ,currentGradientDisplay_);

  updateWidgets();
}

/// Convenience method to set all the widgets correctly.
void KeithleyTemperatureWidget::updateWidgets() {

  State sensorState = model_->getSensorState(sensor_);

  enabledCheckBox_->setChecked(sensorState == READY || sensorState == INITIALIZING);

  if (sensorState == READY) {
    currentTempLabel_->setEnabled( true );
    currentTempDisplay_->setEnabled( true );
    currentGradientLabel_->setEnabled( true );
    currentGradientDisplay_->setEnabled( true );
  } else {
    currentTempLabel_->setEnabled( false );
    currentTempDisplay_->setEnabled( false );
    currentGradientLabel_->setEnabled( false );
    currentGradientDisplay_->setEnabled( false );
  }
}

/// Updates the GUI according to the current device state.
void KeithleyTemperatureWidget::keithleyStateChanged(State newState) {

  enabledCheckBox_->setEnabled(newState == READY || newState == INITIALIZING);

  updateWidgets();
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void KeithleyTemperatureWidget::controlStateChanged(bool enabled) {

  if (enabled) {
    State state = model_->getDeviceState();
    enabledCheckBox_->setEnabled(state == READY
			      || state == INITIALIZING);
    updateWidgets();
  } else {
    enabledCheckBox_->setEnabled(false);
    currentTempLabel_->setEnabled( false );
    currentTempDisplay_->setEnabled(false);
    currentGradientLabel_->setEnabled( false );
    currentGradientDisplay_->setEnabled(false);
  }
}

/// Updates the GUI according to the current sensor state.
void KeithleyTemperatureWidget::sensorStateChanged(unsigned int sensor,
                                                   State /* state */)
{
  if ( sensor_ == sensor )
      updateWidgets();
}

/// Relays the new temperature to the history model.
void KeithleyTemperatureWidget::sensorTemperatureChanged(unsigned int sensor,
                                                         double temperature)
{
    if (sensor_!=sensor) return;
    currentTempDisplay_->display(QString::number(temperature, 'f', 2));
}

/// Relays the new temperature to the history model.
void KeithleyTemperatureWidget::sensorTemperatureGradientChanged(unsigned int sensor,
                                                                 double gradient)
{
    if (sensor_!=sensor) return;
    currentGradientDisplay_->display(QString::number(gradient, 'f', 5));
}

/// Updates the model according to the GUI change.
void KeithleyTemperatureWidget::enabledCheckBoxToggled(bool enabled)
{
  model_->setSensorEnabled(sensor_, enabled);
}

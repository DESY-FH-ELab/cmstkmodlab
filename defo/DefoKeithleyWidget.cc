#include "DefoKeithleyWidget.h"

DefoKeithleyWidget::DefoKeithleyWidget(
    DefoKeithleyModel* model
  , QWidget *parent
)
  :
      QWidget(parent)
    , model_(model)
{

  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  keithleyCheckBox_ = new QCheckBox("Enable multimeter", this);
  layout->addWidget(keithleyCheckBox_);

  sensorControlWidget_= new QWidget(this);
  layout->addWidget(sensorControlWidget_);

  // TODO QGridLayout
  QGridLayout* sensorLayout = new QGridLayout(sensorControlWidget_);
  sensorControlWidget_->setLayout(sensorLayout);

  // Add all the temperature sensor displays
  for ( unsigned int i = 0; i < DefoKeithleyModel::SENSOR_COUNT; ++i ) {
    sensorLayout->addWidget(
            new DefoKeithleyTemperatureWidget(model_, i, this)
          , i/5
          , i%5
    );
  }

  connect(
          keithleyCheckBox_
        , SIGNAL(toggled(bool))
        , model_
        , SLOT(setDeviceEnabled(bool))
  );

  connect(
          model_
        , SIGNAL(deviceStateChanged(State))
        , this
        , SLOT(keithleyStateChanged(State))
  );

  keithleyStateChanged( model_->getDeviceState() );

}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void DefoKeithleyWidget::keithleyStateChanged(State newState) {

  keithleyCheckBox_->setChecked(newState == READY || newState == INITIALIZING);
  sensorControlWidget_->setEnabled(newState == READY);

}


/* DefoTemperatureWidget implementation */

const QString DefoKeithleyTemperatureWidget::LABEL_FORMAT =
    QString("Sensor %1");

DefoKeithleyTemperatureWidget::DefoKeithleyTemperatureWidget(
    DefoKeithleyModel* model
  , unsigned int sensor
  , QWidget *parent
)
  :
    QWidget(parent)
  , model_(model)
  , sensor_(sensor)
{

  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  enabledCheckBox_ = new QCheckBox(LABEL_FORMAT.arg(sensor), this);

  historyModel_ = new DefoTemperatureHistoryModel(this);
  historyWidget_ = new DefoTemperatureHistoryWidget(historyModel_, this);

  connect(
        model_
        , SIGNAL(sensorStateChanged(uint,State))
        , this
        , SLOT(sensorStateChagned(uint,State))
  );

  connect(
          model_
        , SIGNAL(temperatureChanged(uint,double))
        , this
        , SLOT(sensorTemperatureChanged(uint,double))
  );

  connect(
          enabledCheckBox_
        , SIGNAL(toggled(bool))
        , this
        , SLOT(enabledCheckBoxToggled(bool))
  );

  connect(
          model_
        , SIGNAL(deviceStateChanged(State))
        , this
        , SLOT(keithleyStateChanged(State))
  );

  layout->addWidget(enabledCheckBox_);
  layout->addWidget(historyWidget_);

  updateWidgets();

}

/// Convenience method to set all the widgets correctly.
void DefoKeithleyTemperatureWidget::updateWidgets() {

  State sensorState = model_->getSensorState(sensor_);

  enabledCheckBox_->setChecked(
       sensorState == READY
    || sensorState == INITIALIZING
  );

  historyWidget_->setEnabled( sensorState == READY );


}

/// Updates the GUI according to the current device state.
void DefoKeithleyTemperatureWidget::keithleyStateChanged(State state) {
  updateWidgets();
}

/// Updates the GUI according to the current sensor state.
void DefoKeithleyTemperatureWidget::sensorStateChagned(
    unsigned int sensor
  , State state
) {

  if ( sensor_ == sensor )
   updateWidgets();

}

/// Relays the new temperature to the history model.
void DefoKeithleyTemperatureWidget::sensorTemperatureChanged(
    unsigned int sensor
  , double temperature
)
{
  if ( sensor_ == sensor )
    historyModel_->addValue(temperature);
}

/// Updates the model according to the GUI change.
void DefoKeithleyTemperatureWidget::enabledCheckBoxToggled(bool enabled)
{
  model_->setSensorEnabled(sensor_, enabled);
}

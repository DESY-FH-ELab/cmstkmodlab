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

#include <QGroupBox>

#include "MartaSVG.h"
#include "MartaWidget.h"

MartaSVGWidget::MartaSVGWidget(QWidget* parent)
  : QSvgWidget(parent)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  sizePolicy().setHeightForWidth(true);
}

int MartaSVGWidget::heightForWidth(int w) const
{
  return w*500/800;
}

int MartaSVGWidget::widthForHeight(int h) const
{
  return h*800/500;
}

void MartaSVGWidget::resizeEvent(QResizeEvent *event)
{
  QSize s = event->size();
  int h = heightForWidth(s.width());
  if (h<=s.height()) {
    resize(s.width(), h);
  } else {
    int w = widthForHeight(s.height());
    resize(w, s.height());
  }
}

MartaWidget::MartaWidget(MartaModel* model,
			 QWidget *parent)
 : QWidget(parent),
   model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  martaCheckBox_ = new QCheckBox("Enable Marta");
  layout->addWidget(martaCheckBox_);

  svgWidget_ = new MartaSVGWidget(this);
  svgWidget_->setMinimumWidth(800);
  svgWidget_->setMinimumHeight(svgWidget_->heightForWidth(800));
  layout->addWidget(svgWidget_);
  
  operationPanel_ = new QWidget(this);
  QFormLayout* operationLayout = new QFormLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);
  layout->addWidget(operationPanel_);

  temperatureSetPointSpinner_ = new QDoubleSpinBox(operationPanel_);
  temperatureSetPointSpinner_->setSingleStep(1.0);
  temperatureSetPointSpinner_->setKeyboardTracking(false);
  temperatureSetPointSpinner_->setMinimum(-35);
  temperatureSetPointSpinner_->setMaximum(+25);
  temperatureSetPointSpinner_->setDecimals(1);
  temperatureSetPointSpinner_->setAlignment(Qt::AlignRight);

  operationLayout->addRow(QString::fromUtf8("Temperature Setpoint [°C]"),
                          temperatureSetPointSpinner_);

  speedSetPointSpinner_ = new QDoubleSpinBox(operationPanel_);
  speedSetPointSpinner_->setSingleStep(50.0);
  speedSetPointSpinner_->setKeyboardTracking(false);
  speedSetPointSpinner_->setMinimum(500);
  speedSetPointSpinner_->setMaximum(6000);
  speedSetPointSpinner_->setDecimals(0);
  speedSetPointSpinner_->setAlignment(Qt::AlignRight);

  operationLayout->addRow(QString::fromUtf8("Pump Speed [rpm]"),
                          speedSetPointSpinner_);

  flowSetPointSpinner_ = new QDoubleSpinBox(operationPanel_);
  flowSetPointSpinner_->setSingleStep(0.1);
  flowSetPointSpinner_->setKeyboardTracking(false);
  flowSetPointSpinner_->setMinimum(0.1);
  flowSetPointSpinner_->setMaximum(6.0);
  flowSetPointSpinner_->setDecimals(1);
  flowSetPointSpinner_->setAlignment(Qt::AlignRight);

  operationLayout->addRow(QString::fromUtf8("Flow Rate [g/s]"),
                          flowSetPointSpinner_);

  QWidget* control = new QWidget(this);
  QHBoxLayout* hlayout = new QHBoxLayout(this);
  control->setLayout(hlayout);
  
  chillerCheckBox_ = new QCheckBox("Chiller enabled");
  hlayout->addWidget(chillerCheckBox_);
  
  co2CheckBox_ = new QCheckBox("CO2 enabled");
  hlayout->addWidget(co2CheckBox_);
 
  QGroupBox *groupBox = new QGroupBox("Pump Mode");
  fixedSpeedButton_ = new QRadioButton("Fixed Speed");
  fixedFlowButton_ = new QRadioButton("Fixed Flow");
  QHBoxLayout *hbox = new QHBoxLayout;
  hbox->addWidget(fixedSpeedButton_);
  hbox->addWidget(fixedFlowButton_);
  groupBox->setLayout(hbox);
  hlayout->addWidget(groupBox);
  
  operationLayout->addRow("", control);

  /*
  alarmView_ = new QTextEdit(this);
  alarmView_->setReadOnly(true);
  alarmView_->setMaximumHeight(100);
  operationLayout->addRow("Alarms", alarmView_);
  */
  
  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(martaCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  connect(temperatureSetPointSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTemperatureSetpoint(double)));

  connect(speedSetPointSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setSpeedSetpoint(double)));

  connect(flowSetPointSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setFlowSetpoint(double)));

  connect(chillerCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setStartChiller(bool)));

  connect(co2CheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setStartCO2(bool)));

  connect(fixedFlowButton_, SIGNAL(toggled(bool)),
          model_, SLOT(setPumpFixedFlow(bool)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
   Updates the GUI according to the new state of the chiller chiller.
*/
void MartaWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY || newState == INITIALIZING);
  martaCheckBox_->setChecked(ready);
  operationPanel_->setEnabled(ready);
}

void MartaWidget::controlStateChanged(bool enabled)
{
  martaCheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
   Sets the values of all the subelements (except the global enablement)
   according to the model.
*/
void MartaWidget::updateInfo()
{
  if (!temperatureSetPointSpinner_->hasFocus())
    temperatureSetPointSpinner_->setValue(model_->getTemperatureSetpoint());

  if (!speedSetPointSpinner_->hasFocus())
    speedSetPointSpinner_->setValue(model_->getSpeedSetpoint());

  if (!flowSetPointSpinner_->hasFocus())
    flowSetPointSpinner_->setValue(model_->getFlowSetpoint());

  chillerCheckBox_->setChecked(model_->getChillerOn());
  co2CheckBox_->setChecked(model_->getCO2On());

  if (model_->getPumpFixedFlow()) {
    fixedSpeedButton_->setChecked(false);
    fixedFlowButton_->setChecked(true);
  } else {
    fixedSpeedButton_->setChecked(true);
    fixedFlowButton_->setChecked(false);
  }

  QString svg = svgString;

  double value;
  QString s;

  value = model_->getPT03();
  s = QString::number(value, 'f', 1);
  svg.replace("PT03§", s);
  
  svgWidget_->load(svg.toLocal8Bit());

  /*
  QString alarmText;
  const QStringList &alarms = model_->getCurrentAlarms();
  QStringList::const_iterator constIterator;
  for (constIterator = alarms.constBegin();
       constIterator != alarms.constEnd();
       ++constIterator) {
    alarmText += *constIterator;
    alarmText += "\n";
  }
  
  alarmView_->clear();
  alarmView_->setPlainText(alarmText);
  */
}

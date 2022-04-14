/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QGroupBox>
#include <cmath>

#include "MartaSVG.h"
#include "MartaWidget.h"

MartaSVGWidget::MartaSVGWidget(QWidget* parent)
  : QSvgWidget(parent)
{
  buttonAlarmStatus_ = QRect(725, 440, 44, 44);

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

void MartaSVGWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  if (!isEnabled()) return;

  double scale = 800./width();
  QPoint p(scale * event->x(), scale * event->y());

  if (buttonAlarmStatus_.contains(p)) {
    QPoint d = p - buttonAlarmStatus_.center();
    double length = std::sqrt(std::pow(d.x(), 2) + std::pow(d.y(), 2));
    if (length<=buttonAlarmStatus_.width()/2) {
      emit alarmStatusDoubleClicked();
    }
  }
}

MartaAlarmDialog::MartaAlarmDialog(MartaModel* model,
				   QWidget* parent)
 : QDialog(parent),
   model_(model)
{
  setMinimumWidth(600);
  setMinimumHeight(400);

  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);
  
  alarmView_ = new QTextEdit(this);
  alarmView_->setReadOnly(true);

  layout->addWidget(alarmView_);
  
  connect(model_, SIGNAL(alarmsChanged()),
          this, SLOT(updateInfo()));

  updateInfo();
}

void MartaAlarmDialog::updateInfo()
{
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
}

MartaWidget::MartaWidget(MartaModel* model,
                         QWidget *parent)
 : QWidget(parent),
   model_(model),
   alarmDialog_(0)
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
  QHBoxLayout* hlayout = new QHBoxLayout(control);
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
  
  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(martaCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  connect(svgWidget_, SIGNAL(alarmStatusDoubleClicked()),
          this, SLOT(showAlarmDialog()));

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
  svgWidget_->setEnabled(ready);
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
  static QString white("255,255,255");
  static QString red("255,0,0");
  static QString yellow("215,180,0");
  static QString green("120,200,0");

  if (model_->getPumpFixedFlow()) {
    fixedSpeedButton_->setChecked(false);
    fixedFlowButton_->setChecked(true);
    
    speedSetPointSpinner_->setEnabled(false);

    flowSetPointSpinner_->setEnabled(true);
  } else {
    fixedSpeedButton_->setChecked(true);
    fixedFlowButton_->setChecked(false);
    
    speedSetPointSpinner_->setEnabled(true);

    flowSetPointSpinner_->setEnabled(false);
  }

  if (!temperatureSetPointSpinner_->hasFocus())
    temperatureSetPointSpinner_->setValue(model_->getTemperatureSetpoint());

  if (!speedSetPointSpinner_->hasFocus())
    speedSetPointSpinner_->setValue(model_->getSpeedSetpoint());

  if (!flowSetPointSpinner_->hasFocus())
    flowSetPointSpinner_->setValue(model_->getFlowSetpoint());

  chillerCheckBox_->setChecked(model_->getChillerOn());
  co2CheckBox_->setChecked(model_->getCO2On());

  QString svg = MartaSVGString;

  double value;
  QString s;

  value = model_->getPT03();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT03§", s);

  value = model_->getPT05();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT05§", s);
  
  value = model_->getPT01CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT01CO2§", s);

  value = model_->getPT02CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT02CO2§", s);

  value = model_->getPT03CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT03CO2§", s);

  value = model_->getPT04CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT04CO2§", s);

  /*
  // missing in schematics
  value = model_->getPT05CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT05CO2§", s);
  */

  value = model_->getPT06CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT06CO2§", s);

  value = model_->getTT02();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT02§", s);

  value = model_->getTT01CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT01CO2§", s);

  value = model_->getTT02CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT02CO2§", s);

  value = model_->getTT03CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT03CO2§", s);

  value = model_->getTT04CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT04CO2§", s);

  /*
  // missing in schematics
  value = model_->getTT05CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT05CO2§", s);
  */

  value = model_->getTT06CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT06CO2§", s);

  value = model_->getTT07CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§TT07CO2§", s);

  value = model_->getSH05();
  s = QString::number(value, 'f', 1);
  svg.replace("§SH05§", s);

  /*
  // missing in MODBUS
  value = model_->getST05();
  s = QString::number(value, 'f', 1);
  svg.replace("§ST05§", s);
  */

  value = model_->getSC01CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§SC01CO2§", s);

  value = model_->getSC02CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§SC02CO2§", s);

  value = model_->getSC03CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§SC03CO2§", s);

  /*
  // missing in schematics
  value = model_->getSC05CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§SC05CO2§", s);
  */

  value = model_->getSC06CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§SC06CO2§", s);

  // missing in schematics
  value = model_->getdP01CO2();
  s = QString::number(value, 'f', 3);
  svg.replace("§dP01CO2§", s);

  // missing in schematics
  value = model_->getdP02CO2();
  s = QString::number(value, 'f', 3);
  svg.replace("§dP02CO2§", s);

  value = model_->getdP03CO2();
  s = QString::number(value, 'f', 3);
  svg.replace("§dP03CO2§", s);

  /*
  // missing in schematics
  value = model_->getdP04CO2();
  s = QString::number(value, 'f', 3);
  svg.replace("§dP04CO2§", s);
  */

  /*
  // missing in schematics
  value = model_->getdT02CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§dT02CO2§", s);
  */

  value = model_->getdT03CO2();
  s = QString::number(value, 'f', 1);
  svg.replace("§dT03CO2§", s);

  value = model_->getST01CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§ST01CO2§", s);

  value = model_->getST02CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§ST02CO2§", s);

  value = model_->getST03CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§ST03CO2§", s);

  value = model_->getST04CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§ST04CO2§", s);

  value = model_->getFT01CO2();
  s = QString::number(value, 'f', 3);
  svg.replace("§FT01CO2§", s);
  
  switch (model_->getAlarmStatus()) {
    case 0:
      svg.replace("@AlarmColor@", green);
      break;
    case 1:
      svg.replace("@AlarmColor@", yellow);
      break;
    default:
      svg.replace("@AlarmColor@", red);
  }

  if (model_->getChillerOn()) {
  	svg.replace("@R507PumpColor@", green);
  } else {
  	svg.replace("@R507PumpColor@", white);
  }

  if (model_->getCO2On()) {
  	svg.replace("@CO2PumpColor@", green);
  } else {
  	svg.replace("@CO2PumpColor@", white);
  }

  svgWidget_->load(svg.toLocal8Bit());
}

void MartaWidget::showAlarmDialog()
{
  if (!alarmDialog_) {
    alarmDialog_ = new MartaAlarmDialog(model_, this);
  }

  alarmDialog_->show();
  alarmDialog_->raise();
  alarmDialog_->activateWindow();
}

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

#include <QLabel>
#include <QFile>
#include <QTextStream>

#include <ApplicationConfig.h>

#include "ThermoPfeifferWidget.h"

ThermoPfeifferSvgWidget::ThermoPfeifferSvgWidget(QWidget* parent)
    :QSvgWidget(parent) {
   setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
   sizePolicy().setHeightForWidth(true);
}

int ThermoPfeifferSvgWidget::heightForWidth(int w) const {

  return w*650/750;
}

int ThermoPfeifferSvgWidget::widthForHeight(int h) const {

  return h*750/650;
}

void ThermoPfeifferSvgWidget::resizeEvent(QResizeEvent *event) {

  QSize s = event->size();
  int h = heightForWidth(s.width());

  if (h<=s.height()) {
    resize(s.width(), h);
  } else {
    int w = widthForHeight(s.height());
    resize(w, s.height());
  }
}

/**
  \brief Creates a new panel with all the controls and read-outs for the Pfeiffer
  chiller.
  */
ThermoPfeifferWidget::ThermoPfeifferWidget(PfeifferModel* model, QWidget *parent) :
    QWidget(parent), model_(model)
{
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);

    // Create all the nescessary widgets
    pfeifferCheckBox_ = new QCheckBox("Enable vacuum gauges", this);
    layout->addWidget(pfeifferCheckBox_);

    operationPanel_ = new QWidget(this);
    QVBoxLayout* layout2 = new QVBoxLayout();
    operationPanel_->setLayout(layout2);
    layout->addWidget(operationPanel_);

  sketch_ = new ThermoPfeifferSvgWidget(operationPanel_);
  layout2->addWidget(sketch_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(pfeifferCheckBox_, SIGNAL(toggled(bool)),
          model, SLOT(setDeviceEnabled(bool)));

  sketchSource_ = "";
  QString filename(Config::CMSTkModLabBasePath.c_str());
  QFile file(filename + "/share/ThermoSketch.svg");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    sketchSource_ = stream.readAll();
  }

  // Set GUI according to the current chiller state
  updateDeviceState( model_->getDeviceState() );
  updateInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
  */
void ThermoPfeifferWidget::updateDeviceState(State newState) {

  bool ready = (newState == READY);
  pfeifferCheckBox_->setChecked( ready );
  operationPanel_->setEnabled( ready );
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void ThermoPfeifferWidget::controlStateChanged(bool enabled) {
  
  pfeifferCheckBox_->setEnabled(enabled);
  if (enabled) {
    State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
  */
void ThermoPfeifferWidget::updateInfo()
{
    bool changed = false;

    double p = model_->getPressure1();
    if (p!=p1_) {
        p1_ = p;
        changed = true;
    }
    p = model_->getPressure2();
    if (p!=p2_) {
            p2_ = p;
            changed = true;
    }

    if (changed) updateSketch();
}

void ThermoPfeifferWidget::updateSketch() {

  QString tempSketch = sketchSource_;
  QString value;

  char dummy[20];

  if (p1_<1) {
      sprintf(dummy, "%.02e", p1_);
  } else if (p1_<10) {
      sprintf(dummy, "%.01f", p1_);
  } else {
      sprintf(dummy, "%d", (int)p1_);
  }
  value = QString("p1 = %1 mbar").arg(dummy);
  tempSketch.replace("@P1@", value);

  if (p2_<1) {
      sprintf(dummy, "%.02e", p2_);
  } else if (p2_<10) {
      sprintf(dummy, "%.01f", p2_);
  } else {
      sprintf(dummy, "%d", (int)p2_);
  }
  value = QString("p2 = %1 mbar").arg(dummy);
  tempSketch.replace("@P2@", value);

  sketch_->load(tempSketch.toLocal8Bit());
}

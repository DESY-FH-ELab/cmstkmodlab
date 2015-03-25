#include <QBoxLayout>
#include <QGridLayout>
#include <iostream>

#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "DefoCalibrationWidget.h"

DefoCalibrationWidget::DefoCalibrationWidget(DefoCalibrationModel* calibrationModel,
                                             QWidget *parent)
: QWidget(parent),
  calibrationModel_(calibrationModel)
{
  QBoxLayout *layout = new QVBoxLayout();
  setLayout(layout);

  QGridLayout *grid = new QGridLayout();
  QWidget * inputs = new QWidget(this);
  inputs->setLayout(grid);
  layout->addWidget(inputs);

  calibXSpinBox_ = new QDoubleSpinBox(inputs);
  calibXSpinBox_->setPrefix("cx = ");
  calibXSpinBox_->setRange(0.75, 1.25);
  calibXSpinBox_->setDecimals(6);
  calibXSpinBox_->setSingleStep(0.001);
  grid->addWidget(calibXSpinBox_, 0, 0);
  calibXSpinBox_->setValue(calibrationModel_->getCalibX());
  connect(calibXSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(calibXChanged(double)));

  calibYSpinBox_ = new QDoubleSpinBox(inputs);
  calibYSpinBox_->setPrefix("cy = ");
  calibYSpinBox_->setRange(0.75, 1.25);
  calibYSpinBox_->setDecimals(6);
  calibYSpinBox_->setSingleStep(0.001);
  grid->addWidget(calibYSpinBox_, 0, 1);
  calibYSpinBox_->setValue(calibrationModel_->getCalibY());
  connect(calibYSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(calibYChanged(double)));

  connect(calibrationModel_, SIGNAL(calibrationChanged()),
          this, SLOT(calibrationChanged()));
}

void DefoCalibrationWidget::calibXChanged(double v)
{
  calibrationModel_->setCalibX(v);
}

void DefoCalibrationWidget::calibYChanged(double v)
{
  calibrationModel_->setCalibY(v);
}

void DefoCalibrationWidget::calibrationChanged()
{
  calibXSpinBox_->setValue(calibrationModel_->getCalibX());
  calibYSpinBox_->setValue(calibrationModel_->getCalibY());
}

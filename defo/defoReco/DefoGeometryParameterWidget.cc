#include <iostream>

#include <QBoxLayout>
#include <QGridLayout>

#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "DefoGeometryParameterWidget.h"

DefoGeometryParameterWidget::DefoGeometryParameterWidget(DefoGeometryModel* geometryModel,
				       QWidget *parent)
 : QWidget(parent),
   geometryModel_(geometryModel)
{
  QBoxLayout *layout = new QVBoxLayout();
  layout->setContentsMargins(3, 3, 3, 3);
  setLayout(layout);

  QGridLayout *grid = new QGridLayout();
  grid->setContentsMargins(0, 0, 0, 0);
  QWidget * inputs = new QWidget(this);
  inputs->setLayout(grid);
  layout->addWidget(inputs);

  angle1SpinBox_ = new QDoubleSpinBox(inputs);
  angle1SpinBox_->setPrefix("a1 = ");
  angle1SpinBox_->setSuffix(" deg");
  angle1SpinBox_->setRange(10.0, 45.0);
  angle1SpinBox_->setDecimals(2);
  angle1SpinBox_->setSingleStep(0.01);
  grid->addWidget(angle1SpinBox_, 0, 0);
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  connect(angle1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle1Changed(double)));

  angle2SpinBox_ = new QDoubleSpinBox(inputs);
  angle2SpinBox_->setPrefix("a2 = ");
  angle2SpinBox_->setSuffix(" deg");
  angle2SpinBox_->setRange(10.0, 45.0);
  angle2SpinBox_->setDecimals(2);
  angle2SpinBox_->setSingleStep(0.01);
  grid->addWidget(angle2SpinBox_, 0, 1);
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  connect(angle2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle2Changed(double)));

  angle3SpinBox_ = new QDoubleSpinBox(inputs);
  angle3SpinBox_->setPrefix("a3 = ");
  angle3SpinBox_->setSuffix(" deg");
  angle3SpinBox_->setRange(-10.0, 10.0);
  angle3SpinBox_->setDecimals(2);
  angle3SpinBox_->setSingleStep(0.01);
  grid->addWidget(angle3SpinBox_, 0, 2);
  angle3SpinBox_->setValue(geometryModel_->getAngle3());
  connect(angle3SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle3Changed(double)));

  distanceSpinBox_ = new QDoubleSpinBox(inputs);
  distanceSpinBox_->setPrefix("d = ");
  distanceSpinBox_->setSuffix(" mm");
  distanceSpinBox_->setRange(100.0, 999.0);
  distanceSpinBox_->setDecimals(0);
  distanceSpinBox_->setSingleStep(1.0);
  grid->addWidget(distanceSpinBox_, 0, 3);
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  connect(distanceSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(distanceChanged(double)));

  height1SpinBox_ = new QDoubleSpinBox(inputs);
  height1SpinBox_->setPrefix("h1 = ");
  height1SpinBox_->setSuffix(" mm");
  height1SpinBox_->setRange(1000.0, 2000.0);
  height1SpinBox_->setDecimals(0);
  height1SpinBox_->setSingleStep(1.0);
  grid->addWidget(height1SpinBox_, 0, 4);
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  connect(height1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height1Changed(double)));

  height2SpinBox_ = new QDoubleSpinBox(inputs);
  height2SpinBox_->setPrefix("h2 = ");
  height2SpinBox_->setSuffix(" mm");
  height2SpinBox_->setRange(0.0, 500.0);
  height2SpinBox_->setDecimals(0);
  height2SpinBox_->setSingleStep(1.0);
  grid->addWidget(height2SpinBox_, 0, 5);
  height2SpinBox_->setValue(geometryModel_->getHeight2());
  connect(height2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height2Changed(double)));

  //grid->addWidget(new QWidget(inputs), 1, 3);
  //grid->addWidget(new QWidget(inputs), 1, 4);

  connect(geometryModel_, SIGNAL(geometryChanged()),
          this, SLOT(geometryChanged()));
}

void DefoGeometryParameterWidget::angle1Changed(double v)
{
  geometryModel_->setAngle1(v);
}

void DefoGeometryParameterWidget::angle2Changed(double v) 
{
  geometryModel_->setAngle2(v);
}

void DefoGeometryParameterWidget::angle3Changed(double v) 
{
  geometryModel_->setAngle3(v);
}

void DefoGeometryParameterWidget::distanceChanged(double v) 
{
  geometryModel_->setDistance(v);
}

void DefoGeometryParameterWidget::height1Changed(double v) 
{
  geometryModel_->setHeight1(v);
}

void DefoGeometryParameterWidget::height2Changed(double v) 
{
  geometryModel_->setHeight2(v);
}

void DefoGeometryParameterWidget::geometryChanged()
{
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  angle3SpinBox_->setValue(geometryModel_->getAngle3());
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  height2SpinBox_->setValue(geometryModel_->getHeight2());
}

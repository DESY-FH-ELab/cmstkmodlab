#include <QBoxLayout>
#include <QGridLayout>
#include <iostream>

#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "Defo2DSplineInterpolationWidget.h"

Defo2DSplineInterpolationWidget::Defo2DSplineInterpolationWidget(Defo2DSplineInterpolationModel* interpolationModel,
                                                                 QWidget *parent)
: QWidget(parent),
  interpolationModel_(interpolationModel)
{
  QBoxLayout *layout = new QVBoxLayout();
  setLayout(layout);

  QGridLayout *grid = new QGridLayout();
  QWidget * inputs = new QWidget(this);
  inputs->setLayout(grid);
  layout->addWidget(inputs);

  kXSpinBox_ = new QSpinBox(inputs);
  kXSpinBox_->setPrefix("kx = ");
  kXSpinBox_->setRange(1, 5);
  grid->addWidget(kXSpinBox_, 0, 0);
  kXSpinBox_->setValue(interpolationModel_->getKX());
  connect(kXSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(kXChanged(double)));

  kYSpinBox_ = new QSpinBox(inputs);
  kYSpinBox_->setPrefix("ky = ");
  kYSpinBox_->setRange(1, 5);
  grid->addWidget(kYSpinBox_, 0, 1);
  kYSpinBox_->setValue(interpolationModel_->getKY());
  connect(kYSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(kYChanged(double)));

  smoothingSpinBox_ = new QDoubleSpinBox(inputs);
  smoothingSpinBox_->setPrefix("s = ");
  smoothingSpinBox_->setRange(0.0, 10.0);
  smoothingSpinBox_->setDecimals(1);
  smoothingSpinBox_->setSingleStep(0.11);
  grid->addWidget(smoothingSpinBox_, 0, 2);
  smoothingSpinBox_->setValue(interpolationModel_->getSmoothing());
  connect(smoothingSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(calibZChanged(double)));

  connect(interpolationModel_, SIGNAL(interpolationParametersChanged()),
          this, SLOT(parametersChanged()));
}

void Defo2DSplineInterpolationWidget::kXChanged(int v)
{
  interpolationModel_->setKX(v);
}

void Defo2DSplineInterpolationWidget::kYChanged(int v)
{
  interpolationModel_->setKY(v);
}

void Defo2DSplineInterpolationWidget::smoothingChanged(double v)
{
  interpolationModel_->setSmoothing(v);
}

void Defo2DSplineInterpolationWidget::parametersChanged()
{
  kXSpinBox_->setValue(interpolationModel_->getKX());
  kYSpinBox_->setValue(interpolationModel_->getKY());
  smoothingSpinBox_->setValue(interpolationModel_->getSmoothing());
}

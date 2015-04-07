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
  QGridLayout *grid = new QGridLayout();
  grid->setContentsMargins(3, 3, 3, 3);
  setLayout(grid);

  kXSpinBox_ = new QSpinBox(this);
  kXSpinBox_->setPrefix("kx = ");
  kXSpinBox_->setRange(1, 5);
  grid->addWidget(kXSpinBox_, 0, 0);
  kXSpinBox_->setValue(interpolationModel_->getKX());
  connect(kXSpinBox_, SIGNAL(valueChanged(int)),
          this, SLOT(kXChanged(int)));

  kYSpinBox_ = new QSpinBox(this);
  kYSpinBox_->setPrefix("ky = ");
  kYSpinBox_->setRange(1, 5);
  grid->addWidget(kYSpinBox_, 0, 1);
  kYSpinBox_->setValue(interpolationModel_->getKY());
  connect(kYSpinBox_, SIGNAL(valueChanged(int)),
          this, SLOT(kYChanged(int)));

  smoothingSpinBox_ = new QDoubleSpinBox(this);
  smoothingSpinBox_->setPrefix("s = ");
  smoothingSpinBox_->setRange(0.0, 10.0);
  smoothingSpinBox_->setDecimals(1);
  smoothingSpinBox_->setSingleStep(0.1);
  grid->addWidget(smoothingSpinBox_, 0, 2);
  smoothingSpinBox_->setValue(interpolationModel_->getSmoothing());
  connect(smoothingSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(smoothingChanged(double)));

  dXSpinBox_ = new QDoubleSpinBox(this);
  dXSpinBox_->setPrefix("dx = ");
  dXSpinBox_->setSuffix(" mm");
  dXSpinBox_->setRange(1.0, 20.0);
  dXSpinBox_->setDecimals(1);
  dXSpinBox_->setSingleStep(0.1);
  grid->addWidget(dXSpinBox_, 0, 3);
  dXSpinBox_->setValue(interpolationModel_->getDX());
  connect(dXSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(dXChanged(double)));

  dYSpinBox_ = new QDoubleSpinBox(this);
  dYSpinBox_->setPrefix("dy = ");
  dYSpinBox_->setSuffix(" mm");
  dYSpinBox_->setRange(1.0, 20.0);
  dYSpinBox_->setDecimals(1);
  dYSpinBox_->setSingleStep(0.1);
  grid->addWidget(dYSpinBox_, 0, 4);
  dYSpinBox_->setValue(interpolationModel_->getDY());
  connect(dYSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(dYChanged(double)));

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

void Defo2DSplineInterpolationWidget::dXChanged(double v)
{
  interpolationModel_->setDX(v);
}

void Defo2DSplineInterpolationWidget::dYChanged(double v)
{
  interpolationModel_->setDY(v);
}

void Defo2DSplineInterpolationWidget::parametersChanged()
{
  kXSpinBox_->setValue(interpolationModel_->getKX());
  kYSpinBox_->setValue(interpolationModel_->getKY());
  smoothingSpinBox_->setValue(interpolationModel_->getSmoothing());
  dXSpinBox_->setValue(interpolationModel_->getDX());
  dYSpinBox_->setValue(interpolationModel_->getDY());
}

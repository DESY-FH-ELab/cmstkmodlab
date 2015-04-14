#include <QBoxLayout>
#include <QGridLayout>

#include <QToolBox>
#include <QLabel>

#include "ApplicationConfig.h"

#include "DefoReconstructionParameterWidget.h"

DefoReconstructionParameterWidget::DefoReconstructionParameterWidget(DefoGeometryModel* geometryModel,
								     DefoCalibrationModel* calibrationModel,
								     Defo2DSplineInterpolationModel* interpolationModel,
								     QWidget *parent)
  : QWidget(parent),
    geometryModel_(geometryModel),
    calibrationModel_(calibrationModel),
    interpolationModel_(interpolationModel)
{
  QBoxLayout *layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  QToolBox *toolBox = new QToolBox(this);
  toolBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QBoxLayout * vbox;
  QWidget *item;

  item = new QWidget(toolBox);
  vbox = new QVBoxLayout();
  item->setLayout(vbox);

  angle1SpinBox_ = new QDoubleSpinBox(item);
  angle1SpinBox_->setPrefix("a1 = ");
  angle1SpinBox_->setSuffix(" deg");
  angle1SpinBox_->setRange(10.0, 45.0);
  angle1SpinBox_->setDecimals(2);
  angle1SpinBox_->setSingleStep(0.01);
  vbox->addWidget(angle1SpinBox_);
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  connect(angle1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle1Changed(double)));

  angle2SpinBox_ = new QDoubleSpinBox(item);
  angle2SpinBox_->setPrefix("a2 = ");
  angle2SpinBox_->setSuffix(" deg");
  angle2SpinBox_->setRange(10.0, 45.0);
  angle2SpinBox_->setDecimals(2);
  angle2SpinBox_->setSingleStep(0.01);
  vbox->addWidget(angle2SpinBox_);
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  connect(angle2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle2Changed(double)));

  angle3SpinBox_ = new QDoubleSpinBox(item);
  angle3SpinBox_->setPrefix("a3 = ");
  angle3SpinBox_->setSuffix(" deg");
  angle3SpinBox_->setRange(-10.0, 10.0);
  angle3SpinBox_->setDecimals(2);
  angle3SpinBox_->setSingleStep(0.01);
  vbox->addWidget(angle3SpinBox_);
  angle3SpinBox_->setValue(geometryModel_->getAngle3());
  connect(angle3SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle3Changed(double)));

  distanceSpinBox_ = new QDoubleSpinBox(item);
  distanceSpinBox_->setPrefix("d = ");
  distanceSpinBox_->setSuffix(" mm");
  distanceSpinBox_->setRange(100.0, 999.0);
  distanceSpinBox_->setDecimals(0);
  distanceSpinBox_->setSingleStep(1.0);
  vbox->addWidget(distanceSpinBox_);
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  connect(distanceSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(distanceChanged(double)));

  height1SpinBox_ = new QDoubleSpinBox(item);
  height1SpinBox_->setPrefix("h1 = ");
  height1SpinBox_->setSuffix(" mm");
  height1SpinBox_->setRange(1000.0, 2000.0);
  height1SpinBox_->setDecimals(0);
  height1SpinBox_->setSingleStep(1.0);
  vbox->addWidget(height1SpinBox_);
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  connect(height1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height1Changed(double)));

  height2SpinBox_ = new QDoubleSpinBox(item);
  height2SpinBox_->setPrefix("h2 = ");
  height2SpinBox_->setSuffix(" mm");
  height2SpinBox_->setRange(0.0, 500.0);
  height2SpinBox_->setDecimals(0);
  height2SpinBox_->setSingleStep(1.0);
  vbox->addWidget(height2SpinBox_);
  height2SpinBox_->setValue(geometryModel_->getHeight2());
  connect(height2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height2Changed(double)));

  vbox->addWidget(new QWidget(item));

  toolBox->addItem(item, "Geometry");
  
  item = new QWidget(toolBox);
  vbox = new QVBoxLayout();
  item->setLayout(vbox);

  calibXSpinBox_ = new QDoubleSpinBox(item);
  calibXSpinBox_->setPrefix("cx = ");
  calibXSpinBox_->setRange(0.75, 1.25);
  calibXSpinBox_->setDecimals(6);
  calibXSpinBox_->setSingleStep(0.001);
  vbox->addWidget(calibXSpinBox_);
  calibXSpinBox_->setValue(calibrationModel_->getCalibX());
  connect(calibXSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(calibXChanged(double)));

  calibYSpinBox_ = new QDoubleSpinBox(item);
  calibYSpinBox_->setPrefix("cy = ");
  calibYSpinBox_->setRange(0.75, 1.25);
  calibYSpinBox_->setDecimals(6);
  calibYSpinBox_->setSingleStep(0.001);
  vbox->addWidget(calibYSpinBox_);
  calibYSpinBox_->setValue(calibrationModel_->getCalibY());
  connect(calibYSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(calibYChanged(double)));

  calibZxSpinBox_ = new QDoubleSpinBox(item);
  calibZxSpinBox_->setPrefix("czx = ");
  calibZxSpinBox_->setRange(0.75, 1.25);
  calibZxSpinBox_->setDecimals(6);
  calibZxSpinBox_->setSingleStep(0.001);
  vbox->addWidget(calibZxSpinBox_);
  calibZxSpinBox_->setValue(calibrationModel_->getCalibZx());
  connect(calibZxSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(calibZxChanged(double)));

  vbox->addWidget(new QWidget(item));

  toolBox->addItem(item, "Calibration");
  
  item = new QWidget(toolBox);
  vbox = new QVBoxLayout();
  item->setLayout(vbox);

  kXSpinBox_ = new QSpinBox(item);
  kXSpinBox_->setPrefix("kx = ");
  kXSpinBox_->setRange(1, 5);
  vbox->addWidget(kXSpinBox_);
  kXSpinBox_->setValue(interpolationModel_->getKX());
  connect(kXSpinBox_, SIGNAL(valueChanged(int)),
          this, SLOT(kXChanged(int)));

  kYSpinBox_ = new QSpinBox(item);
  kYSpinBox_->setPrefix("ky = ");
  kYSpinBox_->setRange(1, 5);
  vbox->addWidget(kYSpinBox_);
  kYSpinBox_->setValue(interpolationModel_->getKY());
  connect(kYSpinBox_, SIGNAL(valueChanged(int)),
          this, SLOT(kYChanged(int)));

  smoothingSpinBox_ = new QDoubleSpinBox(item);
  smoothingSpinBox_->setPrefix("s = ");
  smoothingSpinBox_->setRange(0.0, 10.0);
  smoothingSpinBox_->setDecimals(1);
  smoothingSpinBox_->setSingleStep(0.1);
  vbox->addWidget(smoothingSpinBox_);
  smoothingSpinBox_->setValue(interpolationModel_->getSmoothing());
  connect(smoothingSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(smoothingChanged(double)));

  nxySpinBox_ = new QDoubleSpinBox(item);
  nxySpinBox_->setPrefix("nxy = ");
  nxySpinBox_->setRange(0.5, 1.5);
  nxySpinBox_->setDecimals(2);
  nxySpinBox_->setSingleStep(0.05);
  vbox->addWidget(nxySpinBox_);
  nxySpinBox_->setValue(interpolationModel_->getNXY());
  connect(nxySpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(nxyChanged(double)));

  dXSpinBox_ = new QDoubleSpinBox(item);
  dXSpinBox_->setPrefix("dx = ");
  dXSpinBox_->setSuffix(" mm");
  dXSpinBox_->setRange(1.0, 20.0);
  dXSpinBox_->setDecimals(1);
  dXSpinBox_->setSingleStep(0.1);
  vbox->addWidget(dXSpinBox_);
  dXSpinBox_->setValue(interpolationModel_->getDX());
  connect(dXSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(dXChanged(double)));

  dYSpinBox_ = new QDoubleSpinBox(item);
  dYSpinBox_->setPrefix("dy = ");
  dYSpinBox_->setSuffix(" mm");
  dYSpinBox_->setRange(1.0, 20.0);
  dYSpinBox_->setDecimals(1);
  dYSpinBox_->setSingleStep(0.1);
  vbox->addWidget(dYSpinBox_);
  dYSpinBox_->setValue(interpolationModel_->getDY());
  connect(dYSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(dYChanged(double)));

  vbox->addWidget(new QWidget(item));

  toolBox->addItem(item, "Interpolation");

  layout->addWidget(toolBox);

  connect(geometryModel_, SIGNAL(geometryChanged()),
          this, SLOT(geometryChanged()));

  connect(calibrationModel_, SIGNAL(calibrationChanged()),
          this, SLOT(calibrationChanged()));

  connect(interpolationModel_, SIGNAL(interpolationParametersChanged()),
          this, SLOT(interpolationParametersChanged()));
}

void DefoReconstructionParameterWidget::angle1Changed(double v)
{
  geometryModel_->setAngle1(v);
}

void DefoReconstructionParameterWidget::angle2Changed(double v) 
{
  geometryModel_->setAngle2(v);
}

void DefoReconstructionParameterWidget::angle3Changed(double v) 
{
  geometryModel_->setAngle3(v);
}

void DefoReconstructionParameterWidget::distanceChanged(double v) 
{
  geometryModel_->setDistance(v);
}

void DefoReconstructionParameterWidget::height1Changed(double v) 
{
  geometryModel_->setHeight1(v);
}

void DefoReconstructionParameterWidget::height2Changed(double v) 
{
  geometryModel_->setHeight2(v);
}

void DefoReconstructionParameterWidget::geometryChanged()
{
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  angle3SpinBox_->setValue(geometryModel_->getAngle3());
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  height2SpinBox_->setValue(geometryModel_->getHeight2());
}

void DefoReconstructionParameterWidget::calibXChanged(double v)
{
  calibrationModel_->setCalibX(v);
}

void DefoReconstructionParameterWidget::calibYChanged(double v)
{
  calibrationModel_->setCalibY(v);
}

void DefoReconstructionParameterWidget::calibZxChanged(double v)
{
  calibrationModel_->setCalibZx(v);
}

void DefoReconstructionParameterWidget::calibZyChanged(double v)
{
  calibrationModel_->setCalibZy(v);
}

void DefoReconstructionParameterWidget::calibrationChanged()
{
  calibXSpinBox_->setValue(calibrationModel_->getCalibX());
  calibYSpinBox_->setValue(calibrationModel_->getCalibY());
  calibZxSpinBox_->setValue(calibrationModel_->getCalibZx());
  calibZySpinBox_->setValue(calibrationModel_->getCalibZy());
}

void DefoReconstructionParameterWidget::kXChanged(int v)
{
  interpolationModel_->setKX(v);
}

void DefoReconstructionParameterWidget::kYChanged(int v)
{
  interpolationModel_->setKY(v);
}

void DefoReconstructionParameterWidget::smoothingChanged(double v)
{
  interpolationModel_->setSmoothing(v);
}

void DefoReconstructionParameterWidget::nxyChanged(double v)
{
  interpolationModel_->setNXY(v);
}

void DefoReconstructionParameterWidget::dXChanged(double v)
{
  interpolationModel_->setDX(v);
}

void DefoReconstructionParameterWidget::dYChanged(double v)
{
  interpolationModel_->setDY(v);
}

void DefoReconstructionParameterWidget::interpolationParametersChanged()
{
  kXSpinBox_->setValue(interpolationModel_->getKX());
  kYSpinBox_->setValue(interpolationModel_->getKY());
  smoothingSpinBox_->setValue(interpolationModel_->getSmoothing());
  nxySpinBox_->setValue(interpolationModel_->getNXY());
  dXSpinBox_->setValue(interpolationModel_->getDX());
  dYSpinBox_->setValue(interpolationModel_->getDY());
}

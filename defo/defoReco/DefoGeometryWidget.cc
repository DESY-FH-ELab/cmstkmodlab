#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "DefoConfig.h"

#include "DefoGeometryWidget.h"

DefoGeometryWidget::DefoGeometryWidget(
     DefoGeometryModel* geometryModel
   , QWidget *parent
) :
    QWidget(parent)
  , geometryModel_(geometryModel)
{
  QBoxLayout *layout = new QVBoxLayout();

  setMinimumSize(QSize(400, 300));
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setLayout(layout);


  connect(
          geometryModel_
        , SIGNAL(geometryChanged())
        , this
        , SLOT(geometryChanged())
  	  );

  sketch_ = new QSvgWidget(this);
  layout->addWidget(sketch_);
  prepareSketch();

  angle1SpinBox_ = new QDoubleSpinBox(this);
  angle1SpinBox_->setPrefix("a1 = ");
  angle1SpinBox_->setSuffix(" deg");
  angle1SpinBox_->setRange(10.0, 45.0);
  angle1SpinBox_->setDecimals(1);
  angle1SpinBox_->setSingleStep(0.1);
  angle1SpinBox_->move(25, 75);
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  connect(angle1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle1Changed(double)));

  angle2SpinBox_ = new QDoubleSpinBox(this);
  angle2SpinBox_->setPrefix("a2 = ");
  angle2SpinBox_->setSuffix(" deg");
  angle2SpinBox_->setRange(10.0, 45.0);
  angle2SpinBox_->setDecimals(1);
  angle2SpinBox_->setSingleStep(0.1);
  angle2SpinBox_->move(430, 70);
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  connect(angle2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle2Changed(double)));

  distanceSpinBox_ = new QDoubleSpinBox(this);
  distanceSpinBox_->setPrefix("d = ");
  distanceSpinBox_->setSuffix(" mm");
  distanceSpinBox_->setRange(100.0, 999.0);
  distanceSpinBox_->setDecimals(0);
  distanceSpinBox_->setSingleStep(1.0);
  distanceSpinBox_->move(330, 40);
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  connect(distanceSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle2Changed(double)));

  heightSpinBox_ = new QDoubleSpinBox(this);
  heightSpinBox_->setPrefix("h = ");
  heightSpinBox_->setSuffix(" mm");
  heightSpinBox_->setRange(1000.0, 2000.0);
  heightSpinBox_->setDecimals(0);
  heightSpinBox_->setSingleStep(1.0);
  heightSpinBox_->move(240, 210);
  heightSpinBox_->setValue(geometryModel_->getHeight());
  connect(heightSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle2Changed(double)));

  connect(geometryModel_,
          SIGNAL(geometryChanged()),
          this,
          SLOT(geometryChanged()));
}

void DefoGeometryWidget::prepareSketch() {

  QString filename(Defo::CMSTkModLabBasePath.c_str());
  QPixmap pix(filename + "/share/ODMSketch.svg");
  sketch_->load(filename + "/share/ODMSketch.svg");
}

void DefoGeometryWidget::angle1Changed(double v) {
  geometryModel_->setAngle1(v);
}

void DefoGeometryWidget::angle2Changed(double v) {
  geometryModel_->setAngle1(v);
}

void DefoGeometryWidget::distanceChanged(double v) {
  geometryModel_->setDistance(v);
}

void DefoGeometryWidget::heightChanged(double v) {
  geometryModel_->setHeight(v);
}

void DefoGeometryWidget::geometryChanged() {
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  heightSpinBox_->setValue(geometryModel_->getHeight());
}

#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "DefoConfig.h"

#include "DefoGeometryWidget.h"

DefoSvgWidget::DefoSvgWidget(QWidget* parent)
    :QSvgWidget(parent) {

}

int DefoSvgWidget::heightForWidth(int w) const {
  return w*410/530;
}

DefoGeometryWidget::DefoGeometryWidget(
     DefoGeometryModel* geometryModel
   , QWidget *parent
) :
    QWidget(parent)
  , geometryModel_(geometryModel)
{
  QBoxLayout *layout = new QVBoxLayout();

  setMinimumSize(QSize(400, 300));
  setLayout(layout);

  connect(
          geometryModel_
        , SIGNAL(geometryChanged())
        , this
        , SLOT(geometryChanged())
  	  );

  sketch_ = new DefoSvgWidget(this);
  QSizePolicy qsp(QSizePolicy::Preferred, QSizePolicy::Preferred);
  qsp.setHeightForWidth(true);
  sketch_->setSizePolicy(qsp);
  layout->addWidget(sketch_);

  QBoxLayout *hlayout = new QHBoxLayout();
  QWidget * inputs = new QWidget(this);
  inputs->setLayout(hlayout);
  layout->addWidget(inputs);

  angle1SpinBox_ = new QDoubleSpinBox(inputs);
  angle1SpinBox_->setPrefix("a1 = ");
  angle1SpinBox_->setSuffix(" deg");
  angle1SpinBox_->setRange(10.0, 45.0);
  angle1SpinBox_->setDecimals(1);
  angle1SpinBox_->setSingleStep(0.1);
  hlayout->addWidget(angle1SpinBox_);
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  connect(angle1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle1Changed(double)));

  angle2SpinBox_ = new QDoubleSpinBox(inputs);
  angle2SpinBox_->setPrefix("a2 = ");
  angle2SpinBox_->setSuffix(" deg");
  angle2SpinBox_->setRange(10.0, 45.0);
  angle2SpinBox_->setDecimals(1);
  angle2SpinBox_->setSingleStep(0.1);
  hlayout->addWidget(angle2SpinBox_);
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  connect(angle2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(angle2Changed(double)));

  distanceSpinBox_ = new QDoubleSpinBox(inputs);
  distanceSpinBox_->setPrefix("d = ");
  distanceSpinBox_->setSuffix(" mm");
  distanceSpinBox_->setRange(100.0, 999.0);
  distanceSpinBox_->setDecimals(0);
  distanceSpinBox_->setSingleStep(1.0);
  hlayout->addWidget(distanceSpinBox_);
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  connect(distanceSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(distanceChanged(double)));

  height1SpinBox_ = new QDoubleSpinBox(inputs);
  height1SpinBox_->setPrefix("h1 = ");
  height1SpinBox_->setSuffix(" mm");
  height1SpinBox_->setRange(1000.0, 2000.0);
  height1SpinBox_->setDecimals(0);
  height1SpinBox_->setSingleStep(1.0);
  hlayout->addWidget(height1SpinBox_);
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  connect(height1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height1Changed(double)));

  height2SpinBox_ = new QDoubleSpinBox(inputs);
  height2SpinBox_->setPrefix("h2 = ");
  height2SpinBox_->setSuffix(" mm");
  height2SpinBox_->setRange(0.0, 500.0);
  height2SpinBox_->setDecimals(0);
  height2SpinBox_->setSingleStep(1.0);
  hlayout->addWidget(height2SpinBox_);
  height2SpinBox_->setValue(geometryModel_->getHeight2());
  connect(height2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height2Changed(double)));

  connect(geometryModel_,
          SIGNAL(geometryChanged()),
          this,
          SLOT(geometryChanged()));

  sketchSource_ = "";
  QString filename(Defo::CMSTkModLabBasePath.c_str());
  QFile file(filename + "/share/ODMSketch.svg");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    sketchSource_ = stream.readAll();
  }
  updateSketch();
}

void DefoGeometryWidget::updateSketch() {

  QString tempSketch = sketchSource_;
  QString value;

  value = QString("a1 = %1 deg").arg(QString().setNum(geometryModel_->getAngle1()));
  tempSketch.replace("@A1@", value);

  value = QString("a2 = %1 deg").arg(QString().setNum(geometryModel_->getAngle2()));
  tempSketch.replace("@A2@", value);

  value = QString("d = %1 mm").arg(QString().setNum(geometryModel_->getDistance()));
  tempSketch.replace("@D@", value);

  value = QString("h1 = %1 mm").arg(QString().setNum(geometryModel_->getHeight1()));
  tempSketch.replace("@H1@", value);

  value = QString("h2 = %1 mm").arg(QString().setNum(geometryModel_->getHeight2()));
  tempSketch.replace("@H2@", value);

  sketch_->load(tempSketch.toLocal8Bit());
}

void DefoGeometryWidget::angle1Changed(double v) {
  geometryModel_->setAngle1(v);
  updateSketch();
}

void DefoGeometryWidget::angle2Changed(double v) {
  geometryModel_->setAngle2(v);
  updateSketch();
}

void DefoGeometryWidget::distanceChanged(double v) {
  geometryModel_->setDistance(v);
  updateSketch();
}

void DefoGeometryWidget::height1Changed(double v) {
  geometryModel_->setHeight1(v);
  updateSketch();
}

void DefoGeometryWidget::height2Changed(double v) {
  geometryModel_->setHeight2(v);
  updateSketch();
}

void DefoGeometryWidget::geometryChanged() {
  angle1SpinBox_->setValue(geometryModel_->getAngle1());
  angle2SpinBox_->setValue(geometryModel_->getAngle2());
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  height2SpinBox_->setValue(geometryModel_->getHeight2());
}

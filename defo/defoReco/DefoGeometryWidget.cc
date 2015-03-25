#include <QBoxLayout>
#include <QGridLayout>
#include <iostream>

#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "DefoGeometryWidget.h"

DefoSvgWidget::DefoSvgWidget(QWidget* parent)
    :QSvgWidget(parent) {
   setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
   sizePolicy().setHeightForWidth(true);
}

int DefoSvgWidget::heightForWidth(int w) const {
  //std::cout << w << "\t" << w*410/530 << std::endl;
  return w*410/530;
}

int DefoSvgWidget::widthForHeight(int h) const {
  //std::cout << h << "\t" << h*530/410 << std::endl;
  return h*530/410;
}

void DefoSvgWidget::resizeEvent(QResizeEvent *event) {
  QSize s = event->size();
  //std::cout << s.width() << "\t" << s.height() << std::endl;
  int h = heightForWidth(s.width());
  if (h<=s.height()) {
    resize(s.width(), h);
  } else {
    int w = widthForHeight(s.height());
    resize(w, s.height());
  }
}

DefoGeometryWidget::DefoGeometryWidget(
     DefoGeometryModel* geometryModel
   , QWidget *parent
) :
    QWidget(parent)
  , geometryModel_(geometryModel)
{
  QBoxLayout *layout = new QVBoxLayout();
  setLayout(layout);

  sketch_ = new DefoSvgWidget(this);
  layout->addWidget(sketch_);

  QGridLayout *grid = new QGridLayout();
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
  angle3SpinBox_->setPrefix("a2 = ");
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
  grid->addWidget(distanceSpinBox_, 1, 0);
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  connect(distanceSpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(distanceChanged(double)));

  height1SpinBox_ = new QDoubleSpinBox(inputs);
  height1SpinBox_->setPrefix("h1 = ");
  height1SpinBox_->setSuffix(" mm");
  height1SpinBox_->setRange(1000.0, 2000.0);
  height1SpinBox_->setDecimals(0);
  height1SpinBox_->setSingleStep(1.0);
  grid->addWidget(height1SpinBox_, 1, 1);
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  connect(height1SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height1Changed(double)));

  height2SpinBox_ = new QDoubleSpinBox(inputs);
  height2SpinBox_->setPrefix("h2 = ");
  height2SpinBox_->setSuffix(" mm");
  height2SpinBox_->setRange(0.0, 500.0);
  height2SpinBox_->setDecimals(0);
  height2SpinBox_->setSingleStep(1.0);
  grid->addWidget(height2SpinBox_, 1, 2);
  height2SpinBox_->setValue(geometryModel_->getHeight2());
  connect(height2SpinBox_, SIGNAL(valueChanged(double)),
          this, SLOT(height2Changed(double)));

  connect(geometryModel_,
          SIGNAL(geometryChanged()),
          this,
          SLOT(geometryChanged()));

  sketchSource_ = "";
  QString filename(Config::CMSTkModLabBasePath.c_str());
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

  value = QString("a3 = %1 deg").arg(QString().setNum(geometryModel_->getAngle3()));
  tempSketch.replace("@A3@", value);

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

void DefoGeometryWidget::angle3Changed(double v) {
  geometryModel_->setAngle3(v);
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
  angle3SpinBox_->setValue(geometryModel_->getAngle3());
  distanceSpinBox_->setValue(geometryModel_->getDistance());
  height1SpinBox_->setValue(geometryModel_->getHeight1());
  height2SpinBox_->setValue(geometryModel_->getHeight2());
}

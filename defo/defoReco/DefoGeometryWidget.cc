#include <iostream>

#include <QBoxLayout>
#include <QGridLayout>

#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "DefoGeometryWidget.h"

DefoSvgWidget::DefoSvgWidget(QWidget* parent)
  : QSvgWidget(parent)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  sizePolicy().setHeightForWidth(true);
}

int DefoSvgWidget::heightForWidth(int w) const
{
  //std::cout << w << "\t" << w*410/530 << std::endl;
  return w*410/530;
}

int DefoSvgWidget::widthForHeight(int h) const
{
  //std::cout << h << "\t" << h*530/410 << std::endl;
  return h*530/410;
}

void DefoSvgWidget::resizeEvent(QResizeEvent *event)
{
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

DefoGeometryWidget::DefoGeometryWidget(DefoGeometryModel* geometryModel,
				       QWidget *parent)
 : QWidget(parent),
   geometryModel_(geometryModel)
{
  QBoxLayout *layout = new QVBoxLayout();
  layout->setContentsMargins(3, 3, 3, 3);
  setLayout(layout);

  sketch_ = new DefoSvgWidget(this);
  layout->addWidget(sketch_);

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

void DefoGeometryWidget::geometryChanged()
{
  updateSketch();
}

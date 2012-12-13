#include <cmath>

#include <QResizeEvent>
#include <QMouseEvent>
#include <QScrollBar>

#include "DefoRecoColorHistoWidget.h"

DefoRecoColorHistoWidget::DefoRecoColorHistoWidget(
    DefoMeasurementListModel* listModel
  , DefoMeasurementSelectionModel* selectionModel
  , DefoColorSelectionModel * colorModel
  , QWidget* parent
) :
    QWidget(parent)
  , listModel_(listModel)
  , selectionModel_(selectionModel)
  , colorModel_(colorModel)
{
  setMinimumSize(250, 250);

  connect(
        listModel_
      , SIGNAL(pointsUpdated(const DefoMeasurement*))
      , this
      , SLOT(pointsUpdated(const DefoMeasurement*))
  );

  connect(
        selectionModel_
      , SIGNAL(selectionChanged(DefoMeasurement*))
      , this
      , SLOT(selectionChanged(DefoMeasurement*))
  );

  connect(
        colorModel_
      , SIGNAL(colorChanged(float,float))
      , this
      , SLOT(colorChanged(float,float))
  );
}

void DefoRecoColorHistoWidget::paintEvent(QPaintEvent *event) {

  const double PI = 3.14159265;

  QWidget::paintEvent(event);

  QPainter painter(this);

  /*
      Rotate (according to camera orientation) and
      save the current state before handing of to child class
      */
  painter.save();

  QPen pen1(Qt::SolidPattern, 1);
  pen1.setColor(QColor(0, 0, 0, 255));
  pen1.setWidth(2);
  QPen pen2(Qt::NoBrush, 1);
  pen2.setColor(QColor(255, 0, 0, 255));
  pen2.setWidth(5);

  painter.setPen(pen1);
  painter.setBrush(Qt::SolidPattern);

  int width = size().width();
  int height = size().height();
  int diameter = std::min(width, height)-20;
  int radius = diameter/2;
  int centerX = width/2;
  int centerY = height/2;
  double H, S, angle;

  painter.drawEllipse(centerX-radius, centerY-radius, diameter, diameter);

  if (selectionModel_->getSelection() != NULL) {

    DefoMeasurement* measurement = selectionModel_->getSelection();
    const DefoPointCollection* points = listModel_->getMeasurementPoints(measurement);

    std::cout << "points: " << points << std::endl;

    if (points && points->size()>0) {

      for (DefoPointCollection::const_iterator it = points->begin();
           it < points->end();
           ++it) {
        QColor c = it->getColor();
        H = c.hsvHueF();
        S = 0.9*radius*c.hsvSaturationF();
        angle = 2.0*PI*H;

        c.setHsv(c.hue(), c.saturation(), 255);
        pen2.setColor(c);
        painter.setPen(pen2);

        painter.drawPoint(centerX + S*std::cos(angle), centerY + S*std::sin(angle));
      }
    }
  }

  QPen pen3(Qt::NoBrush, 1);
  QColor c = colorModel_->getColor();
  pen3.setColor(c);
  pen3.setWidth(1);

  H = c.hsvHueF();
  S = 0.9*radius*c.hsvSaturationF();
  int markerSize =0.9*radius*0.10;
  angle = 2.0*PI*H;

  centerX = centerX + S*std::cos(angle);
  centerY = centerY + S*std::sin(angle);

  painter.setPen(pen3);
  painter.drawEllipse(centerX-markerSize, centerY-markerSize, 2*markerSize, 2*markerSize);

  // Restore own state.
  painter.restore();
}

void DefoRecoColorHistoWidget::mouseMoveEvent(QMouseEvent * e) {

  int width = size().width();
  int height = size().height();
  float diameter = std::min(width, height)-20;
  float maxRadius = 0.9*diameter/2;
  float centerX = width/2;
  float centerY = height/2;

  float x = e->x();
  float y = e->y();

  float dx = (x - centerX)/maxRadius;
  float dy = (y - centerY)/maxRadius;
  float hue = std::atan2(dy, dx)/(2.0*3.14159265);
  if (hue<0) hue += 1.0;
  float saturation = std::sqrt(dx*dx+dy*dy);

  colorModel_->setColor(hue, saturation);
}

void DefoRecoColorHistoWidget::pointsUpdated(const DefoMeasurement* /*measurement*/) {
  //std::cout << "void DefoRecoColorHistoWidget::pointsUpdated(const DefoMeasurement*)" << std::endl;
  update();
}

void DefoRecoColorHistoWidget::selectionChanged(DefoMeasurement* /*measurement*/) {
  //std::cout << "void DefoRecoColorHistoWidget::selectionChanged(DefoMeasurement*)" << std::endl;
  update();
}

void DefoRecoColorHistoWidget::colorChanged(float /*hue*/, float /*saturation*/) {
  //std::cout << "void DefoRecoColorHistoWidget::colorChanged(float hue, float saturation)" << std::endl;
  update();
}

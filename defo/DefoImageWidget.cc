#include "DefoImageWidget.h"

/**
  Minimum size of the widget displaying the currently selected measurement.
  */
QSize DefoImageWidget::MINIMUM_SIZE = QSize(400,300);

DefoImageWidget::DefoImageWidget(DefoSurfaceModel* model, QWidget *parent) :
    QWidget(parent)
{

  model_ = model;

  connect(model, SIGNAL(referenceChanged()), SLOT(referenceChanged()));
  setMinimumSize(MINIMUM_SIZE);

}

void DefoImageWidget::referenceChanged() {
  update();
}

void DefoImageWidget::paintEvent(QPaintEvent *event) {

  // TODO repaint only certain rectangle
  QWidget::paintEvent(event);


  if (model_->hasReferenceMeasurement()) {

    const DefoMeasurement* measurement = model_->getReferenceMeasurement();
    const QImage image = measurement->getImage();

    // TODO Maximal image size, rotation and centering
    QSize size(
            std::max(MINIMUM_SIZE.width(), width())
          , std::max(MINIMUM_SIZE.height(), height())
    );

    const QImage scaledImage = image.scaled(
          size
        , Qt::KeepAspectRatio
    );

    QPainter painter(this);
    QPen pen( Qt::NoBrush, 1 );

    // Draw image
    painter.drawImage( QPoint(0,0), scaledImage );


    DefoPointCollection points = measurement->getPoints();
    double scaling = ((double) scaledImage.width()) / ((double) image.width());
    const int width = 8;

    for ( DefoPointCollection::iterator it = points.begin();
          it < points.end();
          ++it
    ) {
      QColor c = it->getColor();
      c.setHsv( c.hue(), 255, 255 );
      pen.setColor( c );

      int x = round( it->getX() * scaling - width/2 );
      int y = round( it->getY() * scaling - width/2 );

      // Draw point
//      painter.setBrush(Qt::SolidPattern);
      painter.setPen( pen );
      painter.drawEllipse(
              static_cast<int>( x )
            , static_cast<int>( y )
            , width
            , width
      );

      // Draw square
//      pen.setBrush( Qt::NoBrush );
//      painter.setPen( pen );
//      painter.drawRect( x-5, y-5, 10, 10 );
    }

  }
}

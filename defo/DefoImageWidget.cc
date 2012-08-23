#include "DefoImageWidget.h"

/// Minimum size of the widget displaying the currently selected measurement.
const QSize DefoImageWidget::MINIMUM_SIZE = QSize(300,400);

DefoImageWidget::DefoImageWidget(
    DefoMeasurementListModel* model
  , QWidget* parent
) :
    QWidget(parent)
  , measurementListModel_(model)
{

  connect(
          measurementListModel_
        , SIGNAL(selectionChanged(int))
        , SLOT(selectionChanged())
  );
  setMinimumSize(MINIMUM_SIZE);

}

void DefoImageWidget::selectionChanged() {
  update();
}

void DefoImageWidget::paintEvent(QPaintEvent *event) {

  // TODO repaint only certain rectangle
  QWidget::paintEvent(event);

  if (measurementListModel_->getMeasurementCount() > 0) {

    QPainter painter(this);

    /*
      Rotate (according to camera orientation) and
      save the current state before handing of to child class
      */
    painter.rotate(-90);
    painter.save();

    DefoMeasurement measurement = measurementListModel_->getSelection();
    QImage prepared = prepareImage(measurement.getImage());

    // Draw image
    painter.drawImage( QPoint(0,0), prepared );

    // Restore own state.
    painter.restore();

    // POINT DRAWING

//    DefoPointCollection points = measurement->getPoints();
//    QPen pen( Qt::NoBrush, 1 );
//    double scaling = ((double) scaledImage.width()) / ((double) image.width());
//    const int width = 8;

//    for ( DefoPointCollection::iterator it = points.begin();
//          it < points.end();
//          ++it
//    ) {
//      QColor c = it->getColor();
//      c.setHsv( c.hue(), 255, 255 );
//      pen.setColor( c );

//      int x = round( it->getX() * scaling - width/2 );
//      int y = round( it->getY() * scaling - width/2 );

//      // Draw point
////      painter.setBrush(Qt::SolidPattern);
//      painter.setPen( pen );
//      painter.drawEllipse(
//              static_cast<int>( x )
//            , static_cast<int>( y )
//            , width
//            , width
//      );

      // Draw square
//      pen.setBrush( Qt::NoBrush );
//      painter.setPen( pen );
//      painter.drawRect( x-5, y-5, 10, 10 );
//    }

  }
}

/// Returns the prefered size of the image to be drawn.
QSize DefoImageWidget::getImageDrawingSize() const {

  return QSize(
        std::max(MINIMUM_SIZE.width(), width())
      , std::max(MINIMUM_SIZE.height(), height())
  );

}


/*
  CHILD CLASSES
  */

/* RAW IMAGE DISPLAY */
DefoRawImageWidget::DefoRawImageWidget(
    DefoMeasurementListModel *model
  , QWidget *parent
) :
    DefoImageWidget(model, parent)
{}

QImage DefoRawImageWidget::prepareImage(const QImage& image) const {

  // TODO Maximal image size and centering
  return image.scaled(getImageDrawingSize(), Qt::KeepAspectRatio);

}

/* IMAGE THRESHOLDS VISUALISATION */
DefoImageThresholdsWidget::DefoImageThresholdsWidget(
    DefoMeasurementListModel *listModel
  , DefoPointRecognitionModel *recognitionModel
  , QWidget *parent
) :
    DefoImageWidget(listModel, parent)
  , recognitionModel_(recognitionModel)
{}

QImage DefoImageThresholdsWidget::prepareImage(const QImage& image) const {

  QImage copy(image);

  for (int x=0; x<copy.width(); ++x) {
    for (int y=0; y<copy.height(); ++y) {
      int gray = qGray(copy.pixel(x, y));

      // Colour according to brightness
      if (gray > recognitionModel_->getThresholdValue(3))
        copy.setPixel(x, y, 0x0000FF);
      else if (gray > recognitionModel_->getThresholdValue(2))
        copy.setPixel(x, y, 0x00FF00);
      else if (gray > recognitionModel_->getThresholdValue(1))
        copy.setPixel(x, y, 0xFF0000);
      else
        copy.setPixel(x, y, 0);
    }
  }

  return copy.scaled( getImageDrawingSize(), Qt::KeepAspectRatio );

}

void DefoImageThresholdsWidget::thresholdChanged(int threshold, int value) {
  // Don't care about which thresholds or what value, needs to be redone anyway
  update();
}

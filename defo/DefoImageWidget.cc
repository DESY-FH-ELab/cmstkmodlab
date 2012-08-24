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
        , this
        , SLOT(selectionChanged(int))
  );
  setMinimumSize(MINIMUM_SIZE);

}

void DefoImageWidget::selectionChanged(int index) {
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
    painter.save();

    DefoMeasurement measurement = measurementListModel_->getSelection();
    QImage prepared = prepareImage(measurement.getImage());

    painter.translate(prepared.height(), 0);
    painter.rotate(90);

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
QSize DefoImageWidget::getImageDrawingSize(const QImage& image) const {

  QSize currentMax = size();
  currentMax.transpose();

  return QSize(
          std::min(currentMax.width(), image.width())
        , std::min(currentMax.height(), image.height())
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
  return image.scaled(getImageDrawingSize(image), Qt::KeepAspectRatio);

}

/* IMAGE THRESHOLDS VISUALISATION */
DefoImageThresholdsWidget::DefoImageThresholdsWidget(
    DefoMeasurementListModel *listModel
  , DefoPointRecognitionModel *recognitionModel
  , QWidget *parent
) :
    DefoImageWidget(listModel, parent)
  , recognitionModel_(recognitionModel)
{
  connect(
        recognitionModel_
      , SIGNAL(thresholdValueChanged(DefoPointRecognitionModel::Threshold,int))
      , this
      , SLOT(thresholdChanged(DefoPointRecognitionModel::Threshold,int))
  );
}

QImage DefoImageThresholdsWidget::prepareImage(const QImage& image) const {

  // Used cache image instead of rescanning the picture
  return imageCache_.scaled(
          getImageDrawingSize(imageCache_)
        , Qt::KeepAspectRatio
  );

}

void DefoImageThresholdsWidget::thresholdChanged(
    DefoPointRecognitionModel::Threshold threshold
  , int value
) {
  // Don't care about which thresholds or what value, needs to be redone anyway
  updateCache();
  update();
}

void DefoImageThresholdsWidget::selectionChanged(int index) {
  updateCache();
  DefoImageWidget::selectionChanged(index);
}

/// Updates the current image cache.
void DefoImageThresholdsWidget::updateCache() {

  imageCache_ = QImage(measurementListModel_->getSelection().getImage());
  int thres1 = recognitionModel_->getThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_1
  );
  int thres2 = recognitionModel_->getThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_2
  );
  int thres3 = recognitionModel_->getThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_3
  );

  for (int x=0; x<imageCache_.width(); ++x) {
    for (int y=0; y<imageCache_.height(); ++y) {
      int gray = qGray(imageCache_.pixel(x, y));

      // Colour according to brightness
      if (gray > thres3)
        imageCache_.setPixel(x, y, 0x0000FF);
      else if (gray > thres2)
        imageCache_.setPixel(x, y, 0x00FF00);
      else if (gray > thres1)
        imageCache_.setPixel(x, y, 0xFF0000);
      else
        imageCache_.setPixel(x, y, 0);
    }
  }

}

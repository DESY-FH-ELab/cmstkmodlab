#include "DefoImageWidget.h"

///
/// DefoImageBaseWidget
///

/// Minimum size of the widget displaying the currently selected measurement.
const QSize DefoImageBaseWidget::MINIMUM_SIZE = QSize(300,400);

DefoImageBaseWidget::DefoImageBaseWidget(
    QWidget* parent
) :
    QWidget(parent)
{
  setMinimumSize(MINIMUM_SIZE);
}

/// Returns the prefered size of the image to be drawn.
QSize DefoImageBaseWidget::getImageDrawingSize(const QImage& image) const {

  QSize currentMax = size();
  currentMax.transpose();

  return QSize(
          std::min(currentMax.width(), image.width())
        , std::min(currentMax.height(), image.height())
  );

}

///
/// DefoImageWidget
///
DefoImageWidget::DefoImageWidget(
    DefoMeasurementSelectionModel* model
  , QWidget* parent
) :
    DefoImageBaseWidget(parent)
  , selectionModel_(model)
{

  connect(
          selectionModel_
        , SIGNAL(selectionChanged(const DefoMeasurementBase*))
        , this
        , SLOT(selectionChanged(const DefoMeasurementBase*))
  );

}

void DefoImageWidget::selectionChanged(const DefoMeasurementBase* /* measurement */) {
  update();
}

void DefoImageWidget::paintEvent(QPaintEvent *event) {

  // TODO repaint only certain rectangle
  QWidget::paintEvent(event);

  if (selectionModel_->getSelection() != NULL) {

    QPainter painter(this);

    /*
      Rotate (according to camera orientation) and
      save the current state before handing of to child class
      */
    painter.save();

    const DefoMeasurementBase* measurement = selectionModel_->getSelection();
    QImage prepared = prepareImage(measurement->getImage());

    painter.translate(prepared.height(), 0);
    painter.rotate(90);

    // Draw image
    painter.drawImage( QPoint(0,0), prepared );

    // Restore own state.
    painter.restore();

  }
}

///
/// DefoLiveViewImageWidget
///
DefoLiveViewImageWidget::DefoLiveViewImageWidget(
    DefoCameraModel* model
  , QWidget* parent
) :
    DefoImageBaseWidget(parent)
  , cameraModel_(model)
{

  connect(
          cameraModel_
	, SIGNAL(newLiveViewImage(QString))
        , this
        , SLOT(newLiveViewImage(QString))
  );
  setMinimumSize(MINIMUM_SIZE);

}

void DefoLiveViewImageWidget::newLiveViewImage(QString /* location */) {
  update();
}

void DefoLiveViewImageWidget::paintEvent(QPaintEvent *event) {

  // TODO repaint only certain rectangle
  QWidget::paintEvent(event);

  if (cameraModel_->getLastLiveViewPicture().isNull()) return;

  QPainter painter(this);

  /*
    Rotate (according to camera orientation) and
    save the current state before handing of to child class
  */
  painter.save();

  QImage prepared = prepareImage(cameraModel_->getLastLiveViewPicture());

  painter.translate(prepared.height(), 0);
  painter.rotate(90);

  // Draw image
  painter.drawImage( QPoint(0,0), prepared );

  // Restore own state.
  painter.restore();
}

QImage DefoLiveViewImageWidget::prepareImage(const QImage& image) const {
  return image.scaled(getImageDrawingSize(image), Qt::KeepAspectRatio);
}

/*
  CHILD CLASSES
  */

/* RAW IMAGE DISPLAY */
DefoRawImageWidget::DefoRawImageWidget(
    DefoMeasurementSelectionModel *model
  , QWidget *parent
) :
    DefoImageWidget(model, parent)
{}

QImage DefoRawImageWidget::prepareImage(const QImage& image) const {
  return image.scaled(getImageDrawingSize(image), Qt::KeepAspectRatio);
}

/* IMAGE THRESHOLDS VISUALISATION */
DefoImageThresholdsWidget::DefoImageThresholdsWidget(
    DefoMeasurementSelectionModel *selectionModel
  , DefoPointRecognitionModel *recognitionModel
  , QWidget *parent
) :
    DefoImageWidget(selectionModel, parent)
  , recognitionModel_(recognitionModel)
{
  connect(
        recognitionModel_
      , SIGNAL(thresholdValueChanged(DefoPointRecognitionModel::Threshold,int))
      , this
      , SLOT(thresholdChanged(DefoPointRecognitionModel::Threshold,int))
  );

  connect(
        recognitionModel_
      , SIGNAL(controlStateChanged(bool))
      , this
      , SLOT(controlStateChanged(bool))
  );

}

QImage DefoImageThresholdsWidget::prepareImage(const QImage& /* image */) const {

  // Used cache image instead of rescanning the picture
  if ( !imageCache_.isNull() )
    return imageCache_.scaled(
            getImageDrawingSize(imageCache_)
          , Qt::KeepAspectRatio
    );
  else
    return QImage();

}

void DefoImageThresholdsWidget::thresholdChanged(
    DefoPointRecognitionModel::Threshold /* threshold */
  , int /* value */
) {
  // Don't care about which thresholds or what value, needs to be redone anyway
  updateCache();
  update();
}

void DefoImageThresholdsWidget::selectionChanged(
    const DefoMeasurementBase* measurement
) {
  updateCache();
  DefoImageWidget::selectionChanged(measurement);
}

void DefoImageThresholdsWidget::controlStateChanged(bool) {

}

/// Updates the current image cache.
void DefoImageThresholdsWidget::updateCache() {

  const DefoMeasurementBase* measurement = selectionModel_->getSelection();

  if (measurement != NULL) {
    imageCache_ = QImage(selectionModel_->getSelection()->getImage());
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
  else
    imageCache_ = QImage();

}


/* IMAGE POINT RECOGNITION DISPLAY */
DefoImagePointsWidget::DefoImagePointsWidget(
    DefoMeasurementListModel *listModel
  , DefoMeasurementSelectionModel *selectionModel
  , QWidget *parent
) :
    DefoRawImageWidget(selectionModel, parent)
  , listModel_(listModel)
{
  connect(
        listModel_
      , SIGNAL(pointsUpdated(const DefoMeasurementBase*))
      , SLOT(pointsUpdated(const DefoMeasurementBase*))
  );
}

void DefoImagePointsWidget::pointsUpdated(const DefoMeasurementBase * /* measurement */) {
  update();
}

void DefoImagePointsWidget::paintEvent(QPaintEvent *event) {

  DefoRawImageWidget::paintEvent(event);

  const DefoMeasurementBase* measurement = selectionModel_->getSelection();

  if ( measurement != NULL
    && listModel_->getMeasurementPoints(measurement) != NULL
    && listModel_->getMeasurementPoints(measurement)->size() > 0
  ) {

    const DefoPointCollection* points =
        listModel_->getMeasurementPoints(measurement);

    QPainter painter(this);

    painter.save();

    QImage image = selectionModel_->getSelection()->getImage();
    // Get bounding box for drawing area
    QSize drawingSize = getImageDrawingSize(image);
    // Find rescaled image size
    double aspectRatio = ((double) image.width()) / image.height();

    if ( aspectRatio*drawingSize.height() > drawingSize.width() )
      drawingSize.setHeight( drawingSize.width()/aspectRatio );
    else
      drawingSize.setWidth( drawingSize.height()*aspectRatio );

    painter.translate(drawingSize.height(), 0);
    painter.rotate(90);


    QPen pen( Qt::NoBrush, 1 );

    double scaling = ((double) drawingSize.height()) / ((double) image.height());
    const int width = 8;

    for ( DefoPointCollection::const_iterator it = points->begin()
        ; it < points->end()
        ; ++it
    ) {

      QColor c = it->getColor();
      c.setHsv( c.hue(), 255, 255 );
      pen.setColor( c );

      int x = round( it->getX() * scaling - width/2. );
      int y = round( it->getY() * scaling - width/2. );

//      Draw point
      painter.setPen( pen );
      painter.drawEllipse(x, y, width, width);

//       Draw square
//      pen.setBrush( Qt::NoBrush );
//      painter.setPen( pen );
//      painter.drawRect( x, y, width, width );

    }

    painter.restore();

  }

}



#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScrollBar>

#include "DefoRecoImageWidget.h"

DefoRecoImageContentWidget::DefoRecoImageContentWidget(
    DefoMeasurementSelectionModel* model
  , DefoRecoImageZoomModel* zoomModel
  , QWidget* parent
) : 
    QWidget(parent)
  , selectionModel_(model)
  , zoomModel_(zoomModel)
{
  setMinimumSize(parent->size());

  connect(
        selectionModel_
      , SIGNAL(selectionChanged(DefoMeasurement*))
      , this
      , SLOT(selectionChanged(DefoMeasurement*))
  );
}

/// Returns the prefered size of the image to be drawn.
QSize DefoRecoImageContentWidget::getImageDrawingSize(const QImage& image) const {
  return zoomModel_->getZoomedSize(size(), image);
}

QImage DefoRecoImageContentWidget::prepareImage(const QImage& image) const {
  return image.scaled(getImageDrawingSize(image) /*, Qt::KeepAspectRatio */);
}

void DefoRecoImageContentWidget::paintEvent(QPaintEvent *event) {

  // std::cout << "DefoRecoRawImageContentWidget::paintEvent(QPaintEvent *event)" << std::endl;

  // std::cout << event->rect().x() << ", "  << event->rect().y() << ", "  << event->rect().width() << ", "  << event->rect().height() << std::endl;

  // TODO repaint only certain rectangle
  QWidget::paintEvent(event);

  if (selectionModel_->getSelection() != NULL) {

    QPainter painter(this);

    /*
      Rotate (according to camera orientation) and
      save the current state before handing of to child class
      */
    painter.save();

    const DefoMeasurement* measurement = selectionModel_->getSelection();
    QImage prepared = prepareImage(measurement->getImage());

    //painter.translate(prepared.height(), 0);
    //painter.rotate(90);

    // Draw image
    painter.drawImage( QPoint(event->rect().x(), event->rect().y()), prepared , event->rect());

    // Restore own state.
    painter.restore();

  }
}

void DefoRecoImageContentWidget::selectionChanged(DefoMeasurement* /*measurement*/)
{
  update();
}

DefoRecoRawImageContentWidget::DefoRecoRawImageContentWidget(
    DefoMeasurementSelectionModel* model
  , DefoRecoImageZoomModel* zoomModel
  , QWidget* parent
) : 
  DefoRecoImageContentWidget(model, zoomModel, parent)
{

}

DefoRecoROIImageContentWidget::DefoRecoROIImageContentWidget(
    DefoMeasurementSelectionModel* model
  , DefoRecoImageZoomModel* zoomModel
  , DefoROIModel* roiModel
  , QWidget* parent
) : 
    DefoRecoImageContentWidget(model, zoomModel, parent)
  , roiModel_(roiModel)
{

}

void DefoRecoROIImageContentWidget::paintEvent(QPaintEvent *event) {

  // std::cout << "DefoRecoROIImageContentWidget::paintEvent(QPaintEvent *event)" << std::endl;

  // std::cout << event->rect().x() << ", "  << event->rect().y() << ", "  << event->rect().width() << ", "  << event->rect().height() << std::endl;

  // TODO repaint only certain rectangle
  QWidget::paintEvent(event);

  if (selectionModel_->getSelection() != NULL) {

    QPainter painter(this);

    /*
      Rotate (according to camera orientation) and
      save the current state before handing of to child class
      */
    painter.save();

    const DefoMeasurement* measurement = selectionModel_->getSelection();
    QImage prepared = prepareImage(measurement->getImage());

    //painter.translate(prepared.height(), 0);
    //painter.rotate(90);
    
    // Draw image
    painter.drawImage( QPoint(event->rect().x(), event->rect().y()), prepared , event->rect());

    float width = this->width();
    float height = this->height();

    QPen pen1(Qt::NoBrush, 1);
    pen1.setColor(QColor(255, 0, 0, 255));
    pen1.setWidth(1);
    QPen pen2(Qt::NoBrush, 1);
    pen2.setColor(QColor(0, 255, 255, 255));
    pen2.setWidth(1);

    painter.setPen(pen1);
 
    for (int j=1;j<roiModel_->size();++j) {
      const QPointF& p0 = roiModel_->at(j-1);
      const QPointF& p1 = roiModel_->at(j);
      painter.drawLine(p0.x()*width, p0.y()*height,
                       p1.x()*width, p1.y()*height);
    }
    const QPointF& p0 = roiModel_->last();
    const QPointF& p1 = roiModel_->first();
    painter.drawLine(p0.x()*width, p0.y()*height,
                     p1.x()*width, p1.y()*height);
    for (int j=0;j<roiModel_->size();++j) {
      const QPointF& p = roiModel_->at(j);
      if (roiModel_->getSelectedPoint()==j) {
        painter.setPen(pen2);
      } else {
        painter.setPen(pen1);
      }
      painter.drawEllipse(p.x()*width-4, p.y()*height-4, 8, 8);
    }

    // Restore own state.
    painter.restore();
  }
}

DefoRecoImageThresholdsContentWidget::DefoRecoImageThresholdsContentWidget(
    DefoMeasurementSelectionModel* model
  , DefoRecoImageZoomModel* zoomModel
  , DefoPointRecognitionModel* recognitionModel
  , DefoROIModel* roiModel
  , QWidget* parent
) :
    DefoRecoImageContentWidget(model, zoomModel, parent)
  , roiModel_(roiModel)
  , recognitionModel_(recognitionModel)
{
  connect(
        recognitionModel_
      , SIGNAL(thresholdValueChanged(DefoPointRecognitionModel::Threshold,int))
      , this
      , SLOT(thresholdChanged(DefoPointRecognitionModel::Threshold,int))
  );

  connect(
        roiModel_
      , SIGNAL(roiChanged())
      , this
      , SLOT(roiChanged())
  );

  needsUpdate_ = false;
}

bool DefoRecoImageThresholdsContentWidget::event(QEvent* event) {

  if (event->type()==QEvent::Show) updateCache();
  return QWidget::event(event);
}

QImage DefoRecoImageThresholdsContentWidget::prepareImage(const QImage& /*image*/) const {

  if ( !imageCache_.isNull() )
    return imageCache_.scaled(
            getImageDrawingSize(imageCache_)
          , Qt::KeepAspectRatio
    );
  else
    return QImage();
}

void DefoRecoImageThresholdsContentWidget::thresholdChanged(
    DefoPointRecognitionModel::Threshold /* threshold */
  , int /* value */
) {
  // Don't care about which thresholds or what value, needs to be redone anyway
  needsUpdate_ = true;
  updateCache();
  update();
}

void DefoRecoImageThresholdsContentWidget::selectionChanged(
    DefoMeasurement* measurement
) {
  needsUpdate_ = true;
  updateCache();
  DefoRecoImageContentWidget::selectionChanged(measurement);
}

void DefoRecoImageThresholdsContentWidget::roiChanged() {
  needsUpdate_ = true;
  updateCache();
}

/// Updates the current image cache.
void DefoRecoImageThresholdsContentWidget::updateCache() {

  if (!isVisible() || !needsUpdate_) return;

  needsUpdate_ = false;

  const DefoMeasurement* measurement = selectionModel_->getSelection();

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

    QRectF bounds = roiModel_->boundingRect();
    int xmin = bounds.x()*imageCache_.width();
    int xmax = (bounds.x()+bounds.width())*imageCache_.width();
    int ymin = bounds.y()*imageCache_.height();
    int ymax = (bounds.y()+bounds.height())*imageCache_.height();
    for (int x=xmin; x<xmax; ++x) {
      for (int y=ymin; y<ymax; ++y) {

        if (roiModel_->containsPoint(imageCache_.width(), imageCache_.height(),
                                     x, y)) {
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
  }
  else
    imageCache_ = QImage();
}

DefoRecoImagePointsContentWidget::DefoRecoImagePointsContentWidget(
      DefoMeasurementListModel* listModel
    , DefoMeasurementSelectionModel* selectionModel
    , DefoRecoImageZoomModel* zoomModel
    , QWidget *parent
) :
    DefoRecoImageContentWidget(selectionModel, zoomModel, parent)
  , listModel_(listModel)
{
  connect(
        listModel_
      , SIGNAL(pointsUpdated(const DefoMeasurement*))
      , SLOT(pointsUpdated(const DefoMeasurement*))
  );
}

void DefoRecoImagePointsContentWidget::paintEvent(QPaintEvent *event) {

  DefoRecoImageContentWidget::paintEvent(event);

  DefoMeasurement* measurement = selectionModel_->getSelection();

  if ( measurement != NULL
    && listModel_->getMeasurementPoints(measurement) != NULL
    && listModel_->getMeasurementPoints(measurement)->size() > 0
  ) {

    const DefoPointCollection* points =
        listModel_->getMeasurementPoints(measurement);

    QPainter painter(this);

    painter.save();

    QImage image = selectionModel_->getSelection()->getImage();
    QSize drawingSize = getImageDrawingSize(image);

    double scaleX = ((double) drawingSize.width())/image.width();
    double scaleY = ((double) drawingSize.height())/image.height();

    QPen pen( Qt::NoBrush, 1 );

    //double scaling = ((double) drawingSize.height()) / ((double) image.height());
    const int width = 8;

    for ( DefoPointCollection::const_iterator it = points->begin()
        ; it < points->end()
        ; ++it
    ) {

      QColor c = it->getColor();
      c.setHsv( c.hue(), 255, 255 );
      pen.setColor( c );

      int x = round( it->getX() * scaleX - width/2. );
      int y = round( it->getY() * scaleY - width/2. );

//    Draw point
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

void DefoRecoImagePointsContentWidget::pointsUpdated(const DefoMeasurement* /*measurement*/) {
  update();
}

DefoRecoAlignmentImageContentWidget::DefoRecoAlignmentImageContentWidget(
    DefoMeasurementSelectionModel* model
  , DefoRecoImageZoomModel* zoomModel
  , DefoAlignmentModel* alignmentModel
  , QWidget* parent
) :
    DefoRecoImageContentWidget(model, zoomModel, parent)
  , alignmentModel_(alignmentModel)
{

}

void DefoRecoAlignmentImageContentWidget::paintEvent(QPaintEvent *event) {

  // std::cout << "DefoRecoAlignmentImageContentWidget::paintEvent(QPaintEvent *event)" << std::endl;

  // std::cout << event->rect().x() << ", "  << event->rect().y() << ", "  << event->rect().width() << ", "  << event->rect().height() << std::endl;

  // TODO repaint only certain rectangle
  QWidget::paintEvent(event);

  if (selectionModel_->getSelection() != NULL) {

    QPainter painter(this);

    /*
      Rotate (according to camera orientation) and
      save the current state before handing of to child class
      */
    painter.save();

    const DefoMeasurement* measurement = selectionModel_->getSelection();
    QImage prepared = prepareImage(measurement->getImage());

    //painter.translate(prepared.height(), 0);
    //painter.rotate(90);

    // Draw image
    painter.drawImage( QPoint(event->rect().x(), event->rect().y()), prepared , event->rect());

    float width = this->width();
    float height = this->height();

    QPen pen1(Qt::NoBrush, 1);
    pen1.setColor(QColor(255, 0, 0, 255));
    pen1.setWidth(1);
    QPen pen2(Qt::NoBrush, 1);
    pen2.setColor(QColor(0, 255, 255, 255));
    pen2.setWidth(1);

    float xo = alignmentModel_->getOrigin().x()*width;
    float yo = alignmentModel_->getOrigin().y()*height;
    float xt = alignmentModel_->getTip().x()*width;
    float yt = alignmentModel_->getTip().y()*height;

    painter.setPen(pen1);
    painter.drawLine(xo, yo, xt, yt);
    painter.drawLine(xo, yo, xo, yt);

    if (alignmentModel_->getSelectedPoint()==DefoAlignmentModel::Origin) {
      painter.setPen(pen2);
    } else {
      painter.setPen(pen1);
    }
    painter.drawEllipse(xo-4, yo-4, 8, 8);

    if (alignmentModel_->getSelectedPoint()==DefoAlignmentModel::Tip) {
      painter.setPen(pen2);
    } else {
      painter.setPen(pen1);
    }
    painter.drawEllipse(xt-4, yt-4, 8, 8);

    // Restore own state.
    painter.restore();
  }
}

/// Minimum size of the widget displaying the currently selected measurement.
const QSize DefoRecoImageWidget::MINIMUM_SIZE = QSize(300,400);

DefoRecoImageWidget::DefoRecoImageWidget(
    DefoMeasurementSelectionModel* model
  , QWidget* parent
) :
    QScrollArea(parent)
  , selectionModel_(model)
{
  setMinimumSize(MINIMUM_SIZE);
  setBackgroundRole(QPalette::Dark);
  zoomModel_ = new DefoRecoImageZoomModel(this);
  
  connect(
          model
        , SIGNAL(selectionChanged(DefoMeasurement*))
        , this
        , SLOT(selectionChanged(DefoMeasurement*))
  );

  connect(
          zoomModel_
        , SIGNAL(zoomFactorChanged(float, float))
        , this
        , SLOT(zoomFactorChanged(float, float))
  );
}

void DefoRecoImageWidget::resizeEvent(QResizeEvent *event) {
  if (selectionModel_->getSelection() != NULL) {
    const DefoMeasurement* measurement = selectionModel_->getSelection();
    QSize newSize = zoomModel_->getZoomedSize(event->size(), measurement->getImage());
    widget()->resize(newSize);
  } else {
    widget()->resize(event->size());
  }
  QScrollArea::resizeEvent(event);
}

void DefoRecoImageWidget::keyReleaseEvent(QKeyEvent * event) {

  if (!(event->modifiers() & Qt::ShiftModifier)) {
    switch (event->key()) {
    case Qt::Key_0:
      zoomModel_->setZoomFactor(0.0);
      event->accept();
      break;
    case Qt::Key_1:
      zoomModel_->setZoomFactor(1.0);
      event->accept();
      break;
    case Qt::Key_Plus:
      zoomModel_->increaseZoomFactor();
      event->accept();
      break;
    case Qt::Key_Minus:
      zoomModel_->decreaseZoomFactor();
      event->accept();
      break;
    default:
      break;
    }
  }
}

void DefoRecoImageWidget::selectionChanged(DefoMeasurement* /* measurement */) {
  update();
  widget()->update();
}

void DefoRecoImageWidget::zoomFactorChanged(float newScaling, float oldScaling) {

  int width = std::min(size().width(), widget()->size().width());
  int height = std::min(size().height(), widget()->size().height());
  int x = newScaling*(horizontalScrollBar()->value() + width/2)/oldScaling;
  int y = newScaling*(verticalScrollBar()->value() + height/2)/oldScaling;

  if (selectionModel_->getSelection() != NULL) {
    const DefoMeasurement* measurement = selectionModel_->getSelection();
    QSize newSize = zoomModel_->getZoomedSize(size(), measurement->getImage());
    widget()->resize(newSize);
  }

  update();
  widget()->update();

  ensureVisible(x, y, width/2, height/2);
}

DefoRecoRawImageWidget::DefoRecoRawImageWidget(
    DefoMeasurementSelectionModel* model
  , QWidget* parent
) :
    DefoRecoImageWidget(model, parent)
{
  setWidget(new DefoRecoRawImageContentWidget(model, zoomModel_, this));
}

DefoRecoROIImageWidget::DefoRecoROIImageWidget(
    DefoMeasurementSelectionModel* model
  , DefoROIModel* roiModel
  , QWidget* parent
) :
    DefoRecoImageWidget(model, parent)
  , roiModel_(roiModel)
{
  setWidget(new DefoRecoROIImageContentWidget(model, zoomModel_, roiModel_, this));

  connect(
          roiModel_
	, SIGNAL(roiChanged())
        , this
        , SLOT(roiChanged())
  );
}

void DefoRecoROIImageWidget::mouseMoveEvent(QMouseEvent * e)
{
  int currentSelectedPoint = roiModel_->getSelectedPoint();
  if (currentSelectedPoint==-1) return;

  float width = widget()->width();
  float height = widget()->height();

  float x = horizontalScrollBar()->value() + e->x();
  float y = verticalScrollBar()->value() + e->y();

  float newX = x / width;
  float newY = y / height;

  roiModel_->setPoint(currentSelectedPoint, newX, newY);
}

void DefoRecoROIImageWidget::mousePressEvent(QMouseEvent * e)
{
  float width = widget()->width();
  float height = widget()->height();

  int x = horizontalScrollBar()->value() + e->x();
  int y = verticalScrollBar()->value() + e->y();

  int index;
  float distance = roiModel_->getClosestPoint(index,
                                              width, height,
                                              x, y);

  if (distance>5 && index!=-1) return;

  std::cout << index << ": " << distance << std::endl;

  roiModel_->selectPoint(index);

  widget()->update();
}

void DefoRecoROIImageWidget::keyReleaseEvent(QKeyEvent * event) {

  if ((event->modifiers() & Qt::ShiftModifier)) {
    switch (event->key()) {
    case Qt::Key_Plus:
      roiModel_->appendPoint();
      event->accept();
      break;
    case Qt::Key_Minus:
      roiModel_->removePoint();
      event->accept();
      break;
    default:
      break;
    }
  }

  DefoRecoImageWidget::keyReleaseEvent(event);
}

void DefoRecoROIImageWidget::mouseReleaseEvent(QMouseEvent * /*e*/) {
  int currentSelectedPoint = roiModel_->getSelectedPoint();
  roiModel_->selectPoint(-1);
  if (currentSelectedPoint!=-1) widget()->update();
}

void DefoRecoROIImageWidget::roiChanged() {
  widget()->update();
}

DefoRecoImageThresholdsWidget::DefoRecoImageThresholdsWidget(
    DefoMeasurementSelectionModel* model
  , DefoPointRecognitionModel* recognitionModel
  , DefoROIModel* roiModel
  , QWidget* parent
) :
    DefoRecoImageWidget(model, parent)
{
  setWidget(new DefoRecoImageThresholdsContentWidget(model, zoomModel_, recognitionModel, roiModel, this));
}

DefoRecoImagePointsWidget::DefoRecoImagePointsWidget(
    DefoMeasurementListModel *listModel
  , DefoMeasurementSelectionModel *selectionModel
  , QWidget *parent
) :
  DefoRecoImageWidget(selectionModel, parent)
{
  setWidget(new DefoRecoImagePointsContentWidget(listModel, selectionModel, zoomModel_, this));
}

DefoRecoAlignmentImageWidget::DefoRecoAlignmentImageWidget(
    DefoMeasurementSelectionModel* model
  , DefoAlignmentModel* alignmentModel
  , QWidget* parent
) :
    DefoRecoImageWidget(model, parent)
  , alignmentModel_(alignmentModel)
{
  setWidget(new DefoRecoAlignmentImageContentWidget(model, zoomModel_, alignmentModel_, this));

  connect(
          alignmentModel_
        , SIGNAL(alignmentChanged(double))
        , this
        , SLOT(alignmentChanged(double))
  );
}

void DefoRecoAlignmentImageWidget::mouseMoveEvent(QMouseEvent * e)
{
  DefoAlignmentModel::AlignmentPoint currentSelectedPoint = alignmentModel_->getSelectedPoint();
  if (currentSelectedPoint==DefoAlignmentModel::None) return;

  float width = widget()->width();
  float height = widget()->height();

  float x = horizontalScrollBar()->value() + e->x();
  float y = verticalScrollBar()->value() + e->y();

  float newX = x / width;
  float newY = y / height;

  alignmentModel_->setPoint(currentSelectedPoint, newX, newY);
}

void DefoRecoAlignmentImageWidget::mousePressEvent(QMouseEvent * e)
{
  float width = widget()->width();
  float height = widget()->height();

  int x = horizontalScrollBar()->value() + e->x();
  int y = verticalScrollBar()->value() + e->y();

  DefoAlignmentModel::AlignmentPoint point;
  float distance = alignmentModel_->getClosestPoint(point,
                                              width, height,
                                              x, y);

  if (distance>5) return;

  alignmentModel_->selectPoint(point);

  widget()->update();
}

void DefoRecoAlignmentImageWidget::mouseReleaseEvent(QMouseEvent * /*e*/) {
  DefoAlignmentModel::AlignmentPoint currentSelectedPoint = alignmentModel_->getSelectedPoint();
  alignmentModel_->selectPoint(DefoAlignmentModel::None);
  if (currentSelectedPoint!=DefoAlignmentModel::None) widget()->update();
}

void DefoRecoAlignmentImageWidget::alignmentChanged(double /*angle*/) {
  widget()->update();
}

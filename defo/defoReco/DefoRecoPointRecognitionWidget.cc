#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>

#include "DefoThresholdSpinBox.h"
#include "DefoHalfSquareWidthSpinBox.h"
#include "DefoPointFinder.h"
#include "DefoPoint.h"
#include "DefoPointSaver.h"

#include "DefoRecoPointRecognitionWidget.h"

DefoRecoPointRecognitionWidget::DefoRecoPointRecognitionWidget(
    DefoMeasurementListModel* listModel
  , DefoMeasurementSelectionModel* selectionModel
  , DefoPointRecognitionModel* pointModel
  , DefoROIModel* roiModel
  , QWidget *parent
) :
    QTabWidget(parent)
  , listModel_(listModel)
  , selectionModel_(selectionModel)
  , pointModel_(pointModel)
  , roiModel_(roiModel)
{
  // THRESHOLDS
  QWidget* thresholds = new QWidget(this);
  QVBoxLayout* thresholdsLayout = new QVBoxLayout(thresholds);
  thresholds->setLayout(thresholdsLayout);
  addTab(thresholds, "Thresholds");

  thresholdsLayout->addWidget(
      new DefoRecoImageThresholdsWidget(selectionModel_, pointModel_, roiModel_, thresholds)
  );

  QWidget* thresholdSpinners = new QWidget(thresholds);
  thresholdsLayout->addWidget(thresholdSpinners);
  thresholdSpinners->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QHBoxLayout* thresholdLayout = new QHBoxLayout(thresholdSpinners);
  thresholdSpinners->setLayout(thresholdLayout);

  QString format("Thr %1");
  for (int i=0; i<3; i++) {
    thresholdLayout->addWidget(new QLabel(format.arg(i+1)));
    thresholdLayout->addWidget(new DefoThresholdSpinBox(
                                 pointModel_
                               , static_cast<DefoPointRecognitionModel::Threshold>(i)
                                 ));
  }
  thresholdLayout->addWidget(new QLabel("HSW"));
  thresholdLayout->addWidget(new DefoHalfSquareWidthSpinBox(
                                 pointModel_
                            ));

  // POINT FINDING
  QWidget* points = new QWidget(this);
  QVBoxLayout* pointsLayout = new QVBoxLayout(points);
  points->setLayout(pointsLayout);
  addTab(points, "Points");

  DefoRecoImagePointsWidget * pointsImage = new DefoRecoImagePointsWidget(listModel_, selectionModel_, points);
  pointsLayout->addWidget(pointsImage);

  QPushButton* findPoints_ = new QPushButton("Find &points", points);
  pointsLayout->addWidget(findPoints_);
  connect(findPoints_, SIGNAL(clicked()), SLOT(findPointsButtonClicked()));

  QPushButton* savePoints_ = new QPushButton("Save points", points);
  pointsLayout->addWidget(savePoints_);
  connect(savePoints_, SIGNAL(clicked()), SLOT(savePointsButtonClicked()));

  connect(selectionModel_, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(selectionChanged(DefoMeasurement*)));
  
  connect(
        pointModel_
      , SIGNAL(controlStateChanged(bool))
      , this
      , SLOT(controlStateChanged(bool))
  );
}

void DefoRecoPointRecognitionWidget::selectionChanged(DefoMeasurement* /*measurement*/) {

}

void DefoRecoPointRecognitionWidget::findPointsButtonClicked() {

  DefoMeasurement* measurement = selectionModel_->getSelection();
  listModel_->setMeasurementPoints(measurement, NULL);

  DefoPointFinder* finder;

  QRect imageArea = measurement->getImage().rect();
  QRectF roiArea = roiModel_->boundingRect();
  float xmin = roiArea.left();
  float xmax = roiArea.right();
  float ymin = roiArea.top();
  float ymax = roiArea.bottom()+pointModel_->getHalfSquareWidth();

  QRect searchArea(xmin*imageArea.width(),
                   ymin*imageArea.height() - pointModel_->getHalfSquareWidth(),
                   (xmax-xmin)*imageArea.width(),
                   (ymax-ymin)*imageArea.height() + pointModel_->getHalfSquareWidth());

  /*
    The number of blocks depends on the number of cores available and the
    number of cores you want to be available for other tasks.
    */
  const int blocks = 6;
  const int width = searchArea.width() / blocks;

  for (double i = 0; i < blocks; ++i) {

    /*
      In Qt the rectangle borders are placed ON the pixels, in contrast to e.g.
      Swing where they are in between the pixels.
      As a consequence, the right-side border has to be one pixel to the left in
      order to avoid overlapping rectangles.
      */
    QRect area = searchArea;
    area.setX(searchArea.x()+i*width - pointModel_->getHalfSquareWidth());
    area.setWidth(width + 2 * pointModel_->getHalfSquareWidth());

    finder = new DefoPointFinder(
        listModel_
      , pointModel_
      , measurement
      , area
      , roiModel_
    );

    finder->start();
  }
}

void DefoRecoPointRecognitionWidget::savePointsButtonClicked()
{
  QString file = QFileDialog::getSaveFileName(this, "Choose location");

  if (file.length() > 0) {
    DefoPointSaver saver(file);
    DefoMeasurement* meas = selectionModel_->getSelection();
    const DefoPointCollection* points = listModel_->getMeasurementPoints(meas);

    saver.writePoints(*points);
  }
}

void DefoRecoPointRecognitionWidget::controlStateChanged(bool enabled) {

  findPoints_->setEnabled(enabled);
  savePoints_->setEnabled(enabled);
}

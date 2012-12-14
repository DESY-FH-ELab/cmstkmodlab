#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>

#include "DefoThresholdSpinBox.h"
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

  QString format("Threshold %1");
  for (int i=0; i<3; i++) {
    thresholdLayout->addWidget(new QLabel(format.arg(i+1)));
    thresholdLayout->addWidget(new DefoThresholdSpinBox(
                                 pointModel_
                               , static_cast<DefoPointRecognitionModel::Threshold>(i)
                                 ));
  }

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

void DefoRecoPointRecognitionWidget::selectionChanged(DefoMeasurement* measurement) {

  std::cout << measurement << std::endl;
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
  float ymax = roiArea.bottom();

  //std::cout << xmin << std::endl;
  //std::cout << xmax << std::endl;
  //std::cout << ymin << std::endl;
  //std::cout << ymax << std::endl;

  QRect searchArea(xmin*imageArea.width(), ymin*imageArea.height(),
                   (xmax-xmin)*imageArea.width(), (ymax-ymin)*imageArea.height());

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
    area.setX(searchArea.x()+i*width-1);
    area.setWidth(width+1);

    //std::cout << "VER: " << area.y() << ", " << area.height() << std::endl;
    //std::cout << "HOR: " << area.x() << ", " << area.right() << std::endl;

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

    for ( DefoPointCollection::const_iterator it = points->begin()
        ; it < points->end()
        ; ++it
    ) {
      saver.writePoint(*it);
    }
  }
}

void DefoRecoPointRecognitionWidget::controlStateChanged(bool enabled) {

  findPoints_->setEnabled(enabled);
  savePoints_->setEnabled(enabled);
}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>

#include "DefoThresholdSpinBox.h"
#include "DefoHalfSquareWidthSpinBox.h"
#include "DefoPointFinder.h"
#include "DefoPoint.h"
#include "DefoPointSaver.h"

#include "DefoPointRecognitionWidget.h"

DefoPointRecognitionWidget::DefoPointRecognitionWidget(DefoMeasurementListModel* listModel,
                                                       DefoMeasurementSelectionModel* selectionModel,
                                                       DefoPointRecognitionModel* pointModel,
                                                       QWidget *parent)
: QTabWidget(parent),
  listModel_(listModel),
  selectionModel_(selectionModel),
  pointModel_(pointModel)
{
  // THRESHOLDS
  QWidget* thresholds = new QWidget(this);
  QVBoxLayout* thresholdsLayout = new QVBoxLayout(thresholds);
  thresholds->setLayout(thresholdsLayout);
  addTab(thresholds, "Thresholds");

  thresholdsLayout->addWidget(new DefoImageThresholdsWidget(selectionModel_,
                                                            pointModel_,
                                                            thresholds));

  QWidget* thresholdSpinners = new QWidget(thresholds);
  thresholdsLayout->addWidget(thresholdSpinners);
  thresholdSpinners->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QHBoxLayout* thresholdLayout = new QHBoxLayout(thresholdSpinners);
  thresholdSpinners->setLayout(thresholdLayout);

  QString format("Thr %1");
  for (int i=0; i<3; i++) {
    thresholdLayout->addWidget(new QLabel(format.arg(i+1)));
    thresholdLayout->addWidget(new DefoThresholdSpinBox(pointModel_,
                                                        static_cast<DefoPointRecognitionModel::Threshold>(i)));
  }
  thresholdLayout->addWidget(new QLabel("HSW"));
  thresholdLayout->addWidget(new DefoHalfSquareWidthSpinBox(pointModel_));

  // POINT FINDING
  QWidget* points = new QWidget(this);
  QVBoxLayout* pointsLayout = new QVBoxLayout(points);
  points->setLayout(pointsLayout);
  addTab(points, "Points");

  DefoImagePointsWidget * pointsImage = new DefoImagePointsWidget(listModel_,
                                                                  selectionModel_,
                                                                  points);
  pointsLayout->addWidget(pointsImage);

  fitPoints_ = new QCheckBox("Fit 2D gaussian over found points", points);
  pointsLayout->addWidget(fitPoints_);
  fitPoints_->setChecked(false);

  findPoints_ = new QPushButton("Find &points", points);
  pointsLayout->addWidget(findPoints_);
  connect(findPoints_, SIGNAL(clicked()), SLOT(findPointsButtonClicked()));

  savePoints_ = new QPushButton("Save points", points);
  pointsLayout->addWidget(savePoints_);
  connect(savePoints_, SIGNAL(clicked()), SLOT(savePointsButtonClicked()));

  connect(pointModel_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
}

void DefoPointRecognitionWidget::findPointsButtonClicked()
{
  DefoMeasurement* measurement = selectionModel_->getSelection();
  listModel_->setMeasurementPoints(measurement, NULL);

  DefoPointFinder* finder;
  QRect searchArea = measurement->getImage().rect();
  const int width = searchArea.width();

  bool do2Dfit = fitPoints_->isChecked();

  /*
    The number of blocks depends on the number of cores available and the
    number of cores you want to be available for other tasks.
   */
  const int blocks = 6;

  for (double i = 0; i < blocks; ++i) {

    /*
      In Qt the rectangle borders are placed ON the pixels, in contrast to e.g.
      Swing where they are in between the pixels.
      As a consequence, the right-side border has to be one pixel to the left in
      order to avoid overlapping rectangles.
     */
    searchArea.setLeft( i/blocks * width );
    searchArea.setRight( (i+1)/blocks * width - 1 );

    finder = new DefoPointFinder(i,
                                 &mutex_,
                                 listModel_,
                                 pointModel_,
                                 measurement,
                                 searchArea,
                                 do2Dfit);
    
    connect(finder, SIGNAL(finished()),
            finder, SLOT(deleteLater()));

    finder->start();
  }
}

void DefoPointRecognitionWidget::savePointsButtonClicked()
{
  QString file = QFileDialog::getSaveFileName(this, "Choose location");

  if (file.length() > 0) {
    DefoPointSaver saver(file);
    DefoMeasurement* meas = selectionModel_->getSelection();
    const DefoPointCollection* points = listModel_->getMeasurementPoints(meas);

    saver.writePoints(*points);
  }
}

void DefoPointRecognitionWidget::controlStateChanged(bool enabled) {

  setEnabled(enabled);
}

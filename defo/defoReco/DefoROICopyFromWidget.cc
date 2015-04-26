#include <iostream>

#include <QBoxLayout>
#include <QGridLayout>

#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "DefoMeasurementListComboBox.h"
#include "DefoRecoMeasurement.h"
#include "DefoROICopyFromWidget.h"

DefoROICopyFromWidget::DefoROICopyFromWidget(DefoRecoMeasurementListModel *listModel,
                                             DefoRecoROIModel *roiModel,
                                             QWidget *parent)
 : QWidget(parent),
   listModel_(listModel),
   roiModel_(roiModel)
{
  QHBoxLayout * layout = new QHBoxLayout();
  setLayout(layout);

  layout->addWidget(new QLabel("Copy ROI from", this));

  copyFromSelectionModel_ = new DefoMeasurementSelectionModel(this);
  DefoMeasurementListComboBox *cpFromSelect = new DefoMeasurementListComboBox(listModel_,
                                                                              copyFromSelectionModel_,
                                                                              this);
  layout->addWidget(cpFromSelect);

  copyFromButton_ = new QPushButton("Copy", this);
  layout->addWidget(copyFromButton_);
  connect(copyFromButton_, SIGNAL(clicked()),
          this, SLOT(copyFromButtonClicked()));

  //connect(geometryModel_, SIGNAL(geometryChanged()),
  //        this, SLOT(geometryChanged()));
}

void DefoROICopyFromWidget::copyFromButtonClicked()
{
  DefoRecoMeasurement* rm = dynamic_cast<DefoRecoMeasurement*>(copyFromSelectionModel_->getSelection());
  DefoROI *roi = rm->roi();
  roiModel_->assignFrom(roi);
}

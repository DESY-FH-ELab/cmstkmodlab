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
#include "DefoROICopyToWidget.h"

DefoROICopyToWidget::DefoROICopyToWidget(DefoRecoMeasurementListModel *listModel,
                                         DefoRecoROIModel *roiModel,
                                         QWidget *parent)
: QWidget(parent),
  listModel_(listModel),
  roiModel_(roiModel)
{
  QHBoxLayout * layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  layout->addWidget(new QLabel("Copy ROI to", this));

  copyToSelectionModel_ = new DefoMeasurementSelectionModel(this);
  DefoMeasurementListComboBox *cpToSelect = new DefoMeasurementListComboBox(listModel_,
                                                                            copyToSelectionModel_,
                                                                            this);
  layout->addWidget(cpToSelect);

  copyToButton_ = new QPushButton("Copy", this);
  layout->addWidget(copyToButton_);
  connect(copyToButton_, SIGNAL(clicked()),
          this, SLOT(copyToButtonClicked()));
}

void DefoROICopyToWidget::copyToButtonClicked()
{
  DefoRecoMeasurement* rm = dynamic_cast<DefoRecoMeasurement*>(copyToSelectionModel_->getSelection());
  DefoROI *roi = rm->roi();
  roiModel_->assignTo(roi);
}

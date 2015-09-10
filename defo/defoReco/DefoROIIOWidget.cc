#include <iostream>

#include <QBoxLayout>
#include <QGridLayout>

#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "DefoMeasurementListComboBox.h"
#include "DefoRecoMeasurement.h"
#include "DefoROIIOWidget.h"

DefoROIIOWidget::DefoROIIOWidget(DefoRecoMeasurementListModel *listModel,
                                 DefoRecoROIModel *roiModel,
                                 QWidget *parent)
: QWidget(parent),
  listModel_(listModel),
  roiModel_(roiModel)
{
  QHBoxLayout * layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  exportButton_ = new QPushButton("Export", this);
  layout->addWidget(exportButton_);
  connect(exportButton_, SIGNAL(clicked()),
          this, SLOT(exportButtonClicked()));

  importButton_ = new QPushButton("Import", this);
  layout->addWidget(importButton_);
  connect(importButton_, SIGNAL(clicked()),
          this, SLOT(importButtonClicked()));
}

void DefoROIIOWidget::exportButtonClicked()
{
  QString filename = QFileDialog::getSaveFileName(this,
                                                  "export ROI",
                                                  "./",
                                                  "ODM roi (*.odmr)",
                                                  0,
                                                  0);
  if (filename.isNull()) return;
  if (!filename.endsWith(".odmr")) filename += ".odmr";

  roiModel_->write(filename);
}

void DefoROIIOWidget::importButtonClicked()
{
  QString filename = QFileDialog::getOpenFileName(this,
                                                  "import ROI",
                                                  "./",
                                                  "ODM roi (*.odmr)",
                                                  0,
                                                  0);
  if (filename.isNull()) return;
  if (filename.endsWith(".odmr")) {
    roiModel_->read(filename);
  }
}

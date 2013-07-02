#include <QGroupBox>
#include <QVBoxLayout>

#include "ThermoDAQWidget.h"

ThermoDAQWidget::ThermoDAQWidget(ThermoDAQModel* DAQModel, QWidget *parent) :
    QWidget(parent),
    DAQModel_(DAQModel)
{
  // Layouts to put everything into place
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  // Camera buttons
  QGridLayout *buttonLayout = new QGridLayout();
  buttons_ = new QWidget(this);
  buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  buttons_->setLayout(buttonLayout);
  layout->addWidget(buttons_);

  // load a DAQ
  openDAQButton_ = new QPushButton("&Open DAQ", buttons_);
  connect(openDAQButton_, SIGNAL(clicked()), this, SLOT(openDAQButtonClicked()));
  buttonLayout->addWidget(openDAQButton_, 0, 0);

  // save a DAQ
  saveDAQButton_ = new QPushButton("&Save DAQ", buttons_);
  connect(saveDAQButton_, SIGNAL(clicked()), this, SLOT(saveDAQButtonClicked()));
  buttonLayout->addWidget(saveDAQButton_, 0, 1);

  // save a DAQ with new name
  saveAsDAQButton_ = new QPushButton("&Save DAQ as", buttons_);
  connect(saveAsDAQButton_, SIGNAL(clicked()), this, SLOT(saveAsDAQButtonClicked()));
  buttonLayout->addWidget(saveAsDAQButton_, 0, 2);

  // save a DAQ
  executeDAQButton_ = new QPushButton("&Execute DAQ", buttons_);
  connect(executeDAQButton_, SIGNAL(clicked()), this, SLOT(executeDAQButtonClicked()));
  buttonLayout->addWidget(executeDAQButton_, 0, 3);

  // execute DAQ
  abortDAQButton_ = new QPushButton("&Abort DAQ", buttons_);
  connect(abortDAQButton_, SIGNAL(clicked()), this, SLOT(abortDAQButtonClicked()));
  buttonLayout->addWidget(abortDAQButton_, 0, 4);

  temperaturePlot_ = new QwtPlot(QwtText("Temperatures"), this);
  layout->addWidget(temperaturePlot_);

  pressurePlot_ = new QwtPlot(QwtText("Pressures"), this);
  layout->addWidget(pressurePlot_);

  updateGeometry();
}

void ThermoDAQWidget::openDAQButtonClicked() {

//  QString filename = QFileDialog::getOpenFileName(this,
//                                                  "open DAQ",
//                                                  "./",
//                                                  "Thermo DAQs (*.tsr)",
//                                                  0,
//                                                  0);
//  if (!filename.isNull()) DAQModel_->openDAQ(filename);
}

void ThermoDAQWidget::saveDAQButtonClicked() {

//  QString filename = DAQModel_->currentDAQFilename();
//  if (filename.isNull())
//    this->saveAsDAQButtonClicked();
//  else
//    DAQModel_->saveDAQ(filename);
}

void ThermoDAQWidget::saveAsDAQButtonClicked() {

//  QString filename = QFileDialog::getSaveFileName(this,
//                                                  "save DAQ",
//                                                  "./",
//                                                  "Thermo DAQs (*.tsr)",
//                                                  0,
//                                                  0);
//  if (filename.isNull()) return;
//  if (!filename.endsWith(".tsr")) filename += ".tsr";
//  DAQModel_->saveDAQ(filename);
}

void ThermoDAQWidget::executeDAQButtonClicked() {

//  DAQModel_->executeDAQ();
}

void ThermoDAQWidget::abortDAQButtonClicked() {

//  DAQModel_->abortDAQ();
}

#include <QGroupBox>
#include <QFileDialog>

#include "DefoImageWidget.h"

#include "DefoRecoMainWindow.h"

DefoRecoMainWindow::DefoRecoMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  listModel_ = new DefoMeasurementListModel(this);
  selectionModel_ = new DefoMeasurementSelectionModel(this);

  tabWidget_ = new QTabWidget(this);
  tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QBoxLayout *layout = new QVBoxLayout();
  QWidget * measurementWidget = new QWidget(tabWidget_);
  measurementWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  measurementWidget->setLayout(layout);

  QBoxLayout *hbox = new QHBoxLayout();
  QWidget * measurementInfoWidget = new QWidget(measurementWidget);
  measurementInfoWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  measurementInfoWidget->setLayout(hbox);
  layout->addWidget(measurementInfoWidget);

  QPushButton * loadMeasurementButton = new QPushButton("&Load measurement", measurementInfoWidget);
  connect(loadMeasurementButton, SIGNAL(clicked()),
   	  this, SLOT(loadMeasurementButtonClicked()));
  hbox->addWidget(loadMeasurementButton);

  hbox = new QHBoxLayout();
  measurementInfoWidget = new QWidget(measurementWidget);
  measurementInfoWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  measurementInfoWidget->setLayout(hbox);
  layout->addWidget(measurementInfoWidget);

  DefoMeasurementListTreeWidget * treeWidget = new DefoMeasurementListTreeWidget(listModel_,
										 selectionModel_,
										 measurementInfoWidget);
  hbox->addWidget(treeWidget);

  DefoRawImageWidget *rawImage = new DefoRawImageWidget(selectionModel_, measurementInfoWidget);
  hbox->addWidget(rawImage);

  tabWidget_->addTab(measurementWidget, "Measurement");

  setCentralWidget(tabWidget_);
}

void DefoRecoMainWindow::loadMeasurementButtonClicked() {
  
  QString filename = QFileDialog::getOpenFileName(this,
						  QString("Load Measurement"),
						  QString("/home/tkmodlab/Desktop/measurements/"),
						  QString("ODM Measurement Files (*.odmx)"));
  if (filename.isNull()) return;

  listModel_->clear();
  listModel_->read(filename);
}

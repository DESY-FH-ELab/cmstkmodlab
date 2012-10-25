#include <QGroupBox>
#include <QFileDialog>

#include "DefoRecoMainWindow.h"

DefoRecoMainWindow::DefoRecoMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  listModel_ = new DefoMeasurementListModel(this);

  tabWidget_ = new QTabWidget(this);
  tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QWidget * measurementWidget = new QWidget(tabWidget_);

  QPushButton * loadMeasurementButton = new QPushButton("&Load measurement", measurementWidget);
  connect(loadMeasurementButton, SIGNAL(clicked()),
	  this, SLOT(loadMeasurementButtonClicked()));

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

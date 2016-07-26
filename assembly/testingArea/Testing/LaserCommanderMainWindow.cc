#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>
#include <QThread>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "LaserCommanderMainWindow.h"

LaserCommanderMainWindow::LaserCommanderMainWindow(QWidget *parent)
: QMainWindow(parent)
{
    NQLog("LaserCommanderMainWindow") << "[LaserCommanderMainWindow]";
  ApplicationConfig* config = ApplicationConfig::instance();

  connect(QApplication::instance(), SIGNAL(aboutToQuit()),
          this, SLOT(quit()));

  laserModel_ = new LaserModel(config->getValue<std::string>("KeyenceDevice").c_str());
  laserWidget_ = new LaserWidget();

  laserThread_ = new LaserThread(this);
  laserModel_->moveToThread(laserThread_);
  laserThread_->start();

  QWidget *widget = new QWidget(this);

  QVBoxLayout *vlayout = new QVBoxLayout(widget);
  widget->setLayout(vlayout);
  vlayout->addWidget(laserWidget_);

  testButton_ = new QPushButton("start test", this);
  vlayout->addWidget(testButton_);

  setCentralWidget(widget);

  //QThread *thread = new QThread();
  //NQLog("LaserCommanderMainWindow") << "QThread current thread = " << thread->currentThread();
  //laserModel_->moveToThread(thread);
  //laserWidget_->moveToThread(thread);

  connect(testButton_, SIGNAL(clicked()),
          this, SLOT(testManager()));
  
  value = 0;

  //  connect(testButton_, SIGNAL(clicked()),
  //       this, SLOT(updateD()));

  connect(laserModel_, SIGNAL(measurementChanged(double)),
          laserWidget_, SLOT(updateDisplay(double)));

  connect(laserWidget_, SIGNAL(finished()),
          this, SLOT(repaint()));

  QTimer::singleShot(1000, laserModel_, SLOT(setDeviceEnabled()));

  NQLog("LaserCommanderMainWindow") << "main window constructed";
}

LaserCommanderMainWindow::~LaserCommanderMainWindow()
{
  if(laserModel_){ delete laserModel_; laserModel_ = NULL;}
  if(testButton_){ delete testButton_; testButton_ = NULL;}
  if(laserWidget_){ delete laserWidget_; laserWidget_ = NULL;}
  if(laserThread_){ delete laserThread_; laserThread_ = NULL;}
}

void LaserCommanderMainWindow::quit()
{
  NQLog("LaserCommanderMainWindow") << "quit";

  if (laserThread_) {
      laserThread_->quit();
      laserThread_->wait();
  }

  laserModel_->setDeviceEnabled(false);

}

void LaserCommanderMainWindow::updateD()
{
    NQLog("LaserCommanderMainWindow") << "[updateD]";
    laserModel_->setMeasurement(value+1);
    value++;
}

void LaserCommanderMainWindow::testManager()
{
  NQLog("LaserCommanderMainWindow") << "In test suite";


  //emit changed(5);
  //  usleep(5000);
  //emit changed(11);
  //  laserWidget_->display(5);
  //usleep(5000);
  //laserWidget_->display(10);
  //  usleep(5000);
  //laserWidget_->display(11);

  int run = 0;
  while(run < 100)
      {
          laserModel_->getMeasurement();
          usleep(2000);
          std::cout<<"iteration nr = "<<run<<std::endl;
          run++;
      }

  //laserModel_->setMeasurement(14);
  //NQLog("LaserCommanderMainWindow") << "widget value = " <<laserWidget_->value();
  //  this->update();
  //usleep(10000000);
  //laserModel_->setMeasurement(28);
  //NQLog("LaserCommanderMainWindow") << "widget value = " <<laserWidget_->value();
  /*
  for(int i = 0; i < 25; i++){
      NQLog("LaserCommanderMainWindow") << "i = "<<i;
      laserModel_->setMeasurement(i);
      usleep(50000);
      NQLog("LaserCommanderMainWindow") << "after i ="<<i;
  } 
  */

}; 

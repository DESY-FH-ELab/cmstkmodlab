#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "PlasmaMainWindow.h"

PlasmaMainWindow::PlasmaMainWindow(QWidget *parent)
  : QMainWindow(parent)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    NQLog("PlasmaMainWindow") << "SMCI36PORT: " << config->getValue("SMCI36PORT").c_str();

    smci36Model_ = new NanotecSMCI36Model(config->getValue("SMCI36PORT").c_str(),
                                          0.5, 2.5, this);

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    NanotecSMCI36Widget* smci36Widget_ = new NanotecSMCI36Widget(smci36Model_, tabWidget_);
    smci36Widget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(smci36Widget_, "stage");

    setCentralWidget(tabWidget_);
    updateGeometry();

    NQLog("PlasmaMainWindow") << "main window constructed";
}

void PlasmaMainWindow::quit()
{
    NQLog("PlasmaMainWindow") << "quit";

    delete smci36Model_;
}

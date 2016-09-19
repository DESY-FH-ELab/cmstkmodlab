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

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    setCentralWidget(tabWidget_);
    updateGeometry();

    NQLog("PlasmaMainWindow") << "main window constructed";
}

void PlasmaMainWindow::quit()
{
    NQLog("PlasmaMainWindow") << "quit";
}

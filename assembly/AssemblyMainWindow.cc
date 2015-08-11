#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblyMainWindow.h"

AssemblyMainWindow::AssemblyMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    uEyeModel_ = new AssemblyUEyeModel(10, this);

    connect(QApplication::instance(), SIGNAL(aboutToQuit()),
                this, SLOT(quit()));

    setCentralWidget(tabWidget_);
    updateGeometry();

    NQLog("AssemblyMainWindow") << "main window constructed";
}

void AssemblyMainWindow::quit()
{
    NQLog("AssemblyMainWindow") << "quit";
}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <ApplicationConfig.h>
#include <HuberPetiteFleurWidget.h>
#include <HamegWidget.h>
#include <KeithleyWidget.h>
#include <PfeifferWidget.h>
#include <IotaWidget.h>
#include <ArduinoPresWidget.h>

#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *central = new QWidget();
    central->setLayout(layout);

    ApplicationConfig* config = ApplicationConfig::instance();
    
//    // HUBER MODEL
//    huberModel_ = new HuberPetiteFleurModel(config->getValue<std::string>("HuberPetiteFleurDevice").c_str(),
//					    15, this);
//    HuberPetiteFleurWidget* huberWidget = new HuberPetiteFleurWidget(huberModel_, central);
//    huberWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    layout->addWidget(huberWidget);

//    // HAMEG MODEL
//    hamegModel_ = new HamegModel(2, this);
//    HamegWidget* hamegWidget = new HamegWidget(hamegModel_, central);
//    hamegWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    layout->addWidget(hamegWidget);

//    // KEITHLEY MODEL
//    keithleyModel_ = new KeithleyModel("/dev/ttyS5", 10, this);
//    KeithleyWidget* keithleyWidget = new KeithleyWidget(keithleyModel_, central);
//    keithleyWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    layout->addWidget(keithleyWidget);

//    // PFEIFFER MODEL
//    pfeifferModel_ = new PfeifferModel(5, this);
//    PfeifferWidget* pfeifferWidget = new PfeifferWidget(pfeifferModel_, central);
//    pfeifferWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    layout->addWidget(pfeifferWidget);

    // IOTA MODEL
    iotaModel_ = new IotaModel(config->getValue<std::string>("IotaDevice").c_str(),
                               5, this);
    IotaWidget* iotaWidget = new IotaWidget(iotaModel_, central);
    iotaWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(iotaWidget);

    // ARDUINOPRES MODEL
    arduinoPresModel_ = new ArduinoPresModel(config->getValue<std::string>("ArduinoPresDevice").c_str(),
                                             10, this);
    ArduinoPresWidget* arduinoPresWidget = new ArduinoPresWidget(arduinoPresModel_, central);
    arduinoPresWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(arduinoPresWidget);

    setCentralWidget(central);
}

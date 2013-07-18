#include <HuberPetiteFleurWidget.h>
#include <HamegWidget.h>
#include <KeithleyWidget.h>
#include <PfeifferWidget.h>

#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *central = new QWidget();
    central->setLayout(layout);

    // HUBER MODEL
    huberModel_ = new HuberPetiteFleurModel(30, this);
    HuberPetiteFleurWidget* huberWidget = new HuberPetiteFleurWidget(huberModel_, central);
    huberWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(huberWidget);

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

    setCentralWidget(central);
}

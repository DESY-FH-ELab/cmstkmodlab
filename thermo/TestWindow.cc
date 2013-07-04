#include <HamegWidget.h>

#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *central = new QWidget();
    central->setLayout(layout);

    // HAMEG MODEL
    hamegModel_ = new HamegModel(2, central);
    HamegWidget* hamegWidget = new HamegWidget(hamegModel_, this);
    hamegWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  layout->addWidget(hamegWidget);

  setCentralWidget(central);
}

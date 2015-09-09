#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include <nqlogger.h>

#include "AssemblySensorMarkerFinderWidget.h"

AssemblySensorMarkerFinderWidget::AssemblySensorMarkerFinderWidget(AssemblySensorMarkerFinder *finder,
                                                                   QWidget *parent)
    : QToolBox(parent),
      finder_(finder)
{
    QFormLayout* layout;
    QSpinBox* ispin;
    QDoubleSpinBox* dspin;
    QWidget* w;

    w = new QWidget(this);
    layout = new QFormLayout(w);

    ispin = new QSpinBox(w);
    ispin->setMinimum(1);
    ispin->setMaximum(21);
    ispin->setSingleStep(2);
    ispin->setValue(finder_->gaussianBlurKernelSize());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setGaussianBlurKernelSize(int)));
    layout->addRow("Kernel Size:", ispin);

    ispin = new QSpinBox(w);
    ispin->setMinimum(1);
    ispin->setMaximum(9);
    ispin->setSingleStep(2);
    ispin->setValue(finder_->gaussianBlurSigma());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setGaussianBlurSigma(int)));
    layout->addRow("Sigma:", ispin);

    w->setLayout(layout);
    addItem(w, "Gaussian Blur");

    w = new QWidget(this);
    layout = new QFormLayout(w);

    ispin = new QSpinBox(w);
    ispin->setMinimum(5);
    ispin->setMaximum(255);
    ispin->setSingleStep(1);
    ispin->setValue(finder_->circleEdgeDetectionThreshold());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setCircleEdgeDetectionThreshold(int)));
    layout->addRow("Edge Detection Threshold:", ispin);

    ispin = new QSpinBox(w);
    ispin->setMinimum(5);
    ispin->setMaximum(255);
    ispin->setSingleStep(1);
    ispin->setValue(finder_->circleCenterDetectionThreshold());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setCircleCenterDetectionThreshold(int)));
    layout->addRow("Center Detection Threshold:", ispin);

    dspin = new QDoubleSpinBox(w);
    dspin->setMinimum(60);
    dspin->setMaximum(100);
    dspin->setSingleStep(1);
    dspin->setValue(finder_->expectedCircleRadius());
    connect(dspin, SIGNAL(valueChanged(double)),
            finder_, SLOT(setExpectedCircleSize(double)));
    layout->addRow("Expected Circle Radius:", dspin);

    w->setLayout(layout);
    addItem(w, "Circle Detection");
}

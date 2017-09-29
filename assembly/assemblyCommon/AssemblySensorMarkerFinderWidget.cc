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

#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>

#include <nqlogger.h>
#include <IntegerComboBox.h>

#include "AssemblySensorMarkerFinderWidget.h"

AssemblySensorMarkerFinderWidget::AssemblySensorMarkerFinderWidget(AssemblySensorMarkerFinder *finder,
                                                                   QWidget *parent)
    : QToolBox(parent),
      finder_(finder)
{
    QFormLayout* layout;
    QSpinBox* ispin;
    QDoubleSpinBox* dspin;
    QCheckBox* checkbox;
    IntegerComboBox* icb;
    QWidget* w;

    w = new QWidget(this);
    layout = new QFormLayout(w);

    icb = new IntegerComboBox(w);
    icb->addItem(0);
    icb->addItem(1);
    icb->addItem(3);
    icb->addItem(5);
    icb->addItem(7);
    icb->addItem(9);
    icb->addItem(11);
    icb->addItem(13);
    icb->addItem(15);
    icb->setCurrentIndex(icb->findData(QVariant(finder_->gaussianBlurKernelSize())));
    connect(icb, SIGNAL(valueChanged(int)),
            finder_, SLOT(setGaussianBlurKernelSize(int)));
    layout->addRow("Kernel Size [pixel]:", icb);

    ispin = new QSpinBox(w);
    ispin->setMinimum(0);
    ispin->setMaximum(9);
    ispin->setSingleStep(1);
    ispin->setValue(finder_->gaussianBlurSigma());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setGaussianBlurSigma(int)));
    layout->addRow("Sigma [pixel]:", ispin);

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
    layout->addRow("Expected Circle Radius [pixel]:", dspin);

    w->setLayout(layout);
    addItem(w, "Circle Detection");

    w = new QWidget(this);
    layout = new QFormLayout(w);

    ispin = new QSpinBox(w);
    ispin->setMinimum(5);
    ispin->setMaximum(255);
    ispin->setSingleStep(1);
    ispin->setValue(finder_->linesCannyEdgeDetectionThreshold1());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setLinesCannyEdgeDetectionThreshold1(int)));
    layout->addRow("Canny Threshold 1:", ispin);

    ispin = new QSpinBox(w);
    ispin->setMinimum(5);
    ispin->setMaximum(255);
    ispin->setSingleStep(1);
    ispin->setValue(finder_->linesCannyEdgeDetectionThreshold2());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setLinesCannyEdgeDetectionThreshold2(int)));
    layout->addRow("Canny Threshold 2:", ispin);

    icb = new IntegerComboBox(w);
    icb->addItem(1);
    icb->addItem(3);
    icb->addItem(5);
    icb->addItem(7);
    icb->setCurrentIndex(icb->findData(QVariant(finder_->linesCannyEdgeDetectionApertureSize())));
    connect(icb, SIGNAL(valueChanged(int)),
            finder_, SLOT(setLinesCannyEdgeDetectionApertureSize(int)));
    layout->addRow("Canny Aperture Size:", icb);

    checkbox = new QCheckBox(w);
    checkbox->setChecked(finder_->linesCannyEdgeDetectionL2Gradient());
    connect(checkbox, SIGNAL(clicked(bool)),
            finder_, SLOT(setLinesCannyEdgeDetectionL2Gradient(bool)));
    layout->addRow("Canny L2 Gradient:", checkbox);

    dspin = new QDoubleSpinBox(w);
    dspin->setMinimum(1);
    dspin->setMaximum(100);
    dspin->setSingleStep(0.1);
    dspin->setValue(finder_->linesHoughDistanceResolution());
    connect(dspin, SIGNAL(valueChanged(double)),
            finder_, SLOT(setLinesHoughDistanceResolution(double)));
    layout->addRow("Hough Line Detection Distance Resolution [pixel]:", dspin);

    dspin = new QDoubleSpinBox(w);
    dspin->setMinimum(0.1);
    dspin->setMaximum(10.0);
    dspin->setSingleStep(0.1);
    dspin->setValue(finder_->linesHoughAngleResolution());
    connect(dspin, SIGNAL(valueChanged(double)),
            finder_, SLOT(setLinesHoughAngleResolution(double)));
    layout->addRow("Hough Line Detection Angle Resolution [deg]:", dspin);

    ispin = new QSpinBox(w);
    ispin->setMinimum(5);
    ispin->setMaximum(255);
    ispin->setSingleStep(1);
    ispin->setValue(finder_->linesHoughThreshold());
    connect(ispin, SIGNAL(valueChanged(int)),
            finder_, SLOT(setLinesHoughThreshold(int)));
    layout->addRow("Hough Line Detection Threshold:", ispin);

    dspin = new QDoubleSpinBox(w);
    dspin->setMinimum(10);
    dspin->setMaximum(200);
    dspin->setSingleStep(0.1);
    dspin->setValue(finder_->linesHoughMinLineLength());
    connect(dspin, SIGNAL(valueChanged(double)),
            finder_, SLOT(setLinesHoughMinLineLength(double)));
    layout->addRow("Hough Line Detection Minimum Line Length:", dspin);

    dspin = new QDoubleSpinBox(w);
    dspin->setMinimum(10);
    dspin->setMaximum(200);
    dspin->setSingleStep(0.1);
    dspin->setValue(finder_->linesHoughMaxLineGap());
    connect(dspin, SIGNAL(valueChanged(double)),
            finder_, SLOT(setLinesHoughHoughMaxLineGap(double)));
    layout->addRow("Hough Line Detection Maximum Line Gap:", dspin);

    w->setLayout(layout);
    addItem(w, "Line Detection");
}

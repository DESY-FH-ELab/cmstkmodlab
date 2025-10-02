/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>

#include "LaserControlsWidget.h"

LaserControlsWidget::LaserControlsWidget(LaserModel* model,
			     QWidget* parent)
    : QWidget(parent),
      laserModel_(model)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    checkBoxEnableLaser_ = new QCheckBox("Enable Laser", this);
    layout->addWidget(checkBoxEnableLaser_);

    QHBoxLayout *samplingLayout = new QHBoxLayout(this);
    layout->addLayout(samplingLayout);
    QHBoxLayout *averagingLayout = new QHBoxLayout(this);
    layout->addLayout(averagingLayout);
    QHBoxLayout *materialLayout = new QHBoxLayout(this);
    layout->addLayout(materialLayout);
    QHBoxLayout *diffuseLayout = new QHBoxLayout(this);
    layout->addLayout(diffuseLayout);

    QLabel *samplingLabel_ = new QLabel("sampling rate");
    samplingBox_ = new QComboBox(this);
    samplingBox_->addItem("20 microsec");
    samplingBox_->addItem("50 microsec");
    samplingBox_->addItem("100 microsec");
    samplingBox_->addItem("200 microsec");
    samplingBox_->addItem("500 microsec");
    samplingBox_->addItem("1000 microsec");
    samplingLayout->addWidget(samplingLabel_);
    samplingLayout->addWidget(samplingBox_);
    QLabel *averagingLabel_ = new QLabel("averaging rate");
    averagingBox_ = new QComboBox(this);
    averagingBox_->addItem("1");
    averagingBox_->addItem("4");
    averagingBox_->addItem("16");
    averagingBox_->addItem("64");
    averagingBox_->addItem("256");
    averagingBox_->addItem("1024");
    averagingBox_->addItem("4096");
    averagingBox_->addItem("16384");
    averagingBox_->addItem("65536");
    averagingBox_->addItem("262144");
    averagingLayout->addWidget(averagingLabel_);
    averagingLayout->addWidget(averagingBox_);
    QLabel *materialLabel_ = new QLabel("material mode");
    materialBox_ = new QComboBox(this);
    materialBox_->addItem("normal");
    materialBox_->addItem("translucent 1");
    materialBox_->addItem("translucent 2");
    materialBox_->addItem("translucent 3");
    materialBox_->addItem("multi-reflective");
    materialLayout->addWidget(materialLabel_);
    materialLayout->addWidget(materialBox_);
    QLabel *diffuseLabel_ = new QLabel("diffuse mode");
    diffuseBox_ = new QComboBox(this);
    diffuseBox_->addItem("normal");
    diffuseBox_->addItem("mirror reflection");
    diffuseLayout->addWidget(diffuseLabel_);
    diffuseLayout->addWidget(diffuseBox_);

    connect(samplingBox_, SIGNAL(currentIndexChanged(int)),
	laserModel_, SLOT(setSamplingRate(int)));
    connect(averagingBox_, SIGNAL(currentIndexChanged(int)),
	laserModel_, SLOT(setAveragingRate(int)));
    connect(materialBox_, SIGNAL(currentIndexChanged(int)),
	laserModel_, SLOT(setMaterialMode(int)));
    connect(diffuseBox_, SIGNAL(currentIndexChanged(int)),
	laserModel_, SLOT(setDiffuseMode(int)));
    connect(checkBoxEnableLaser_, SIGNAL(toggled(bool)),
            laserModel_, SLOT(setDeviceEnabled(bool)));

    connect(laserModel_, SIGNAL(deviceStateChanged(State)),
	this, SLOT(laserStateChanged(State)));

}

void LaserControlsWidget::laserStateChanged(State newState)
{
    checkBoxEnableLaser_->setChecked(newState == READY || newState == INITIALIZING  );
}

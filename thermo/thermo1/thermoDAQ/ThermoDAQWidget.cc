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

#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include "ThermoDAQWidget.h"

ThermoDAQWidget::ThermoDAQWidget(ThermoDAQModel* daqModel, QWidget *parent) :
    QWidget(parent),
    daqModel_(daqModel)
{
    // Layouts to put everything into place
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttons_ = new QWidget(this);
    //buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    buttons_->setLayout(buttonLayout);
    layout->addWidget(buttons_);

    // start a measurement
    startMeasurementButton_ = new QPushButton("Start Measurement", buttons_);
    connect(startMeasurementButton_, SIGNAL(clicked()), daqModel_, SLOT(startMeasurement()));
    buttonLayout->addWidget(startMeasurementButton_);

    // stop a measurement
    stopMeasurementButton_ = new QPushButton("Stop Measurement", buttons_);
    connect(stopMeasurementButton_, SIGNAL(clicked()), daqModel_, SLOT(stopMeasurement()));
    connect(stopMeasurementButton_, SIGNAL(clicked()), this, SLOT(stopMeasurement()));
    buttonLayout->addWidget(stopMeasurementButton_);

    buttonLayout->addSpacing(200);

    // clear history
    clearHistoryButton_ = new QPushButton("Clear History", buttons_);
    connect(clearHistoryButton_, SIGNAL(clicked()), daqModel_, SLOT(clearHistory()));
    buttonLayout->addWidget(clearHistoryButton_);

    QFormLayout *metadataLayout = new QFormLayout();
    metadata_ = new QWidget(this);
    //metadata_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    metadata_->setLayout(metadataLayout);

    sampleThickness_ = new QDoubleSpinBox(metadata_);
    sampleThickness_->setKeyboardTracking(false);
    connect(sampleThickness_, SIGNAL(valueChanged(double)),
            this, SLOT(sampleThicknessChanged(double)));
    metadataLayout->addRow("sample thickness [mm]", sampleThickness_);

    sampleArea_ = new QDoubleSpinBox(metadata_);
    sampleArea_->setKeyboardTracking(false);
    connect(sampleArea_, SIGNAL(valueChanged(double)),
            this, SLOT(sampleAreaChanged(double)));
    metadataLayout->addRow("sample area [mm^2]", sampleArea_);

    layout->addWidget(metadata_);

    logEdit_ = new QPlainTextEdit(this);
    logEdit_->setMinimumWidth(600);
    logEdit_->setMaximumHeight(100);
    layout->addWidget(logEdit_);

    // log button
    logButton_ = new QPushButton("Add Log Message", this);
    connect(logButton_, SIGNAL(clicked()), this, SLOT(logButtonClicked()));
    layout->addWidget(logButton_);

    updateGeometry();

    connect(daqModel_, SIGNAL(daqStateChanged(bool)),
            this, SLOT(daqStateChanged(bool)));
    daqStateChanged(false);
}

void ThermoDAQWidget::daqStateChanged(bool running)
{
    if (running) {
        startMeasurementButton_->setEnabled(false);
        stopMeasurementButton_->setEnabled(true);
    } else {
        if (daqModel_->isMetadataValid()) {
            startMeasurementButton_->setEnabled(true);
        } else {
            startMeasurementButton_->setEnabled(false);
        }
        stopMeasurementButton_->setEnabled(false);
    }
}

void ThermoDAQWidget::logButtonClicked()
{
    QDateTime& utime = daqModel_->currentTime();

    QString buffer;
    QXmlStreamWriter xml(&buffer);

    xml.writeStartElement("Log");
    xml.writeAttribute("time", utime.toString());
    
    QString message = logEdit_->toPlainText();
    message.replace('\n', "&#xA;");
    message.replace('\r', "&#xD;");
    xml.writeCharacters(message);
    
    xml.writeEndElement();

    daqModel_->customDAQMessage(buffer);

    logEdit_->setPlainText("");
}

void ThermoDAQWidget::stopMeasurement()
{
    sampleThickness_->setValue(0.0);
    sampleArea_->setValue(0.0);
}

void ThermoDAQWidget::sampleThicknessChanged(double value)
{
    daqModel_->sampleThicknessChanged(value);
    NQLog("ThermoDAQWidget", NQLog::Spam) << " sampleThicknessChanged: " << value;

    daqStateChanged(daqModel_->daqState());
}

void ThermoDAQWidget::sampleAreaChanged(double value)
{
    daqModel_->sampleAreaChanged(value);
    NQLog("ThermoDAQWidget", NQLog::Spam) << " sampleAreaChanged: " << value;

    daqStateChanged(daqModel_->daqState());
}

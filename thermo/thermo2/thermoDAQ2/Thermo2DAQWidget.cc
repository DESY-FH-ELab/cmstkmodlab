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

#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include "Thermo2DAQWidget.h"

Thermo2DAQWidget::Thermo2DAQWidget(const QString& title,
                                   Thermo2DAQModel* daqModel, QWidget *parent)
  : QGroupBox(title, parent),
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
  startMeasurementButton_ = new QPushButton("Start DAQ", buttons_);
  connect(startMeasurementButton_, SIGNAL(clicked()), daqModel_, SLOT(startMeasurement()));
  buttonLayout->addWidget(startMeasurementButton_);

  // stop a measurement
  stopMeasurementButton_ = new QPushButton("Stop DAQ", buttons_);
  connect(stopMeasurementButton_, SIGNAL(clicked()), daqModel_, SLOT(stopMeasurement()));
  connect(stopMeasurementButton_, SIGNAL(clicked()), this, SLOT(stopMeasurement()));
  buttonLayout->addWidget(stopMeasurementButton_);

  buttonLayout->addSpacing(100);

  // clear history
  clearHistoryButton_ = new QPushButton("Clear History", buttons_);
  connect(clearHistoryButton_, SIGNAL(clicked()), daqModel_, SLOT(clearHistory()));
  buttonLayout->addWidget(clearHistoryButton_);

  logEdit_ = new QPlainTextEdit(this);
  logEdit_->setMinimumWidth(600);
  logEdit_->setMinimumHeight(100);
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

void Thermo2DAQWidget::daqStateChanged(bool running)
{
  if (running) {
    startMeasurementButton_->setEnabled(false);
    stopMeasurementButton_->setEnabled(true);
  } else {
    startMeasurementButton_->setEnabled(true);
    stopMeasurementButton_->setEnabled(false);
  }
}

void Thermo2DAQWidget::logButtonClicked()
{
  QString message = logEdit_->toPlainText();
  if (message.length()==0) return;

  QDateTime& utime = daqModel_->currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);

  xml.writeStartElement("Log");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));

  message.replace('\n', "&#xA;");
  message.replace('\r', "&#xD;");
  xml.writeCharacters(message);

  xml.writeEndElement();

  daqModel_->customDAQMessage(buffer);

  logEdit_->setPlainText("");
}

void Thermo2DAQWidget::stopMeasurement()
{

}

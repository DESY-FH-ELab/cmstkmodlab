#include <QGroupBox>
#include <QVBoxLayout>
#include <QXmlStreamWriter>

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
  buttonLayout->addWidget(stopMeasurementButton_);

  buttonLayout->addSpacing(200);

  // clear history
  clearHistoryButton_ = new QPushButton("Clear History", buttons_);
  connect(clearHistoryButton_, SIGNAL(clicked()), daqModel_, SLOT(clearHistory()));
  buttonLayout->addWidget(clearHistoryButton_);

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
        startMeasurementButton_->setEnabled(true);
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

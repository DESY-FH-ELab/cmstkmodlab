/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <qwt_date.h>

#include <nqlogger.h>

#include "Thermo2DAQModel.h"

Thermo2DAQModel::Thermo2DAQModel(RohdeSchwarzNGE103BModel* nge103BModel,
                                 KeithleyDAQ6510Model* keithleyModel,
                                 QObject *parent)
  : QObject(),
    daqState_(false),
    nge103BModel_(nge103BModel),
    keithleyModel_(keithleyModel)
{
  currentTime_ = QDateTime::currentDateTime();

  connect(nge103BModel_, SIGNAL(informationChanged()),
          this, SLOT(nge103BInfoChanged()));

  connect(keithleyModel_, SIGNAL(informationChanged()),
          this, SLOT(keithleyInfoChanged()));
}

void Thermo2DAQModel::myMoveToThread(QThread *thread)
{
  nge103BModel_->moveToThread(thread);

  this->moveToThread(thread);
}

void Thermo2DAQModel::startMeasurement()
{
  daqState_ = true;
  emit daqStateChanged(true);

  for (int i=0;i<3;++i) {
    nge103BOutputState_[i] = false;
    nge103BOutputMode_[i] = 0;
    nge103BVoltage_[i] = -1.0;
    nge103BCurrent_[i] = -1.0;
    nge103BMeasuredVoltage_[i] = 0.;
    nge103BMeasuredCurrent_[i] = 0.;
  }

  for (unsigned int card=0;card<2;++card) {
    for (unsigned int channel=0;channel<10;++channel) {
      keithleyState_[card][channel] = false;
      keithleyTemperature_[card][channel] = 0.;
    }
  }

  QString buffer;
  createDAQStatusMessage(buffer, true);
  emit daqMessage(buffer);

  NQLogMessage("thermo2DAQ") << "measurement started";
}

void Thermo2DAQModel::createDAQStatusMessage(QString &buffer, bool start)
{
  QDateTime& utime = currentTime();

  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  //
  // Start of Rohde & Schwarz NGE103B
  //
  xml.writeStartElement("RohdeSchwarzNGE103B");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));
  for (int i=0;i<3;++i) {
    xml.writeStartElement(QString("RohdeSchwarzNGE103BChannel"));
    xml.writeAttribute("id", QString::number(i+1));
    xml.writeAttribute("State", nge103BModel_->getOutputState(i+1)==true ? "1" : "0");
    xml.writeAttribute("Mode", QString::number(nge103BModel_->getOutputMode(i+1)));
    xml.writeAttribute("U", QString::number(nge103BModel_->getVoltage(i+1), 'f', 3));
    xml.writeAttribute("mU", QString::number(nge103BModel_->getMeasuredVoltage(i+1), 'f', 3));
    xml.writeAttribute("I", QString::number(nge103BModel_->getCurrent(i+1), 'f', 3));
    xml.writeAttribute("mI", QString::number(nge103BModel_->getMeasuredCurrent(i+1), 'f', 3));
    xml.writeEndElement();
  }
  xml.writeEndElement();
  //
  // End of Rohde & Schwarz NGE103B
  //

  //
  // Start of Keithley DAQ6510
  //
  xml.writeStartElement("KeithleyDAQ6510");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));
  for (unsigned int card=1;card<=2;++card) {
    for (unsigned int channel=1;channel<=10;++channel) {
      unsigned int sensor = card*100 + channel;
      xml.writeStartElement(QString("KeithleyDAQ6510Sensor"));
      xml.writeAttribute("id", QString::number(sensor));
      xml.writeAttribute("State", keithleyModel_->getSensorState(sensor)==READY ? "1" : "0");
      xml.writeAttribute("T", QString::number(keithleyModel_->getTemperature(sensor), 'f', 4));
      xml.writeEndElement();
    }
  }
  xml.writeEndElement();
  //
  // End of Keithley DAQ6510
  //

  if (start) {
    xml.writeStartElement("DAQStarted");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeEndElement();
  }
}

void Thermo2DAQModel::stopMeasurement()
{
  daqState_ = false;
  emit daqStateChanged(false);

  NQLogMessage("thermo2DAQ") << "measurement stopped";
}

void Thermo2DAQModel::clearHistory()
{

}

QDateTime& Thermo2DAQModel::currentTime()
{
  QDateTime ct = QDateTime::currentDateTime();
  if (currentTime_.secsTo(ct)>2) currentTime_ = ct;
  return currentTime_;
}

void Thermo2DAQModel::customDAQMessage(const QString & message)
{
  emit daqMessage(message);
}

void Thermo2DAQModel::nge103BInfoChanged()
{
  NQLogDebug("Thermo2DAQModel") << "nge103BInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  xml.writeStartElement("RohdeSchwarzNGE103B");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));

  for (int i=0;i<3;++i) {

    bool changed = false;
    changed |= updateIfChanged<bool>(nge103BOutputState_[i], nge103BModel_->getOutputState(i+1));
    changed |= updateIfChanged<unsigned int>(nge103BOutputMode_[i], nge103BModel_->getOutputMode(i+1));
    changed |= updateIfChanged<float>(nge103BVoltage_[i], nge103BModel_->getVoltage(i+1));
    changed |= updateIfChanged<float>(nge103BCurrent_[i], nge103BModel_->getCurrent(i+1));
    changed |= updateIfChanged<float>(nge103BMeasuredVoltage_[i], nge103BModel_->getMeasuredVoltage(i+1));
    changed |= updateIfChanged<float>(nge103BMeasuredCurrent_[i], nge103BModel_->getMeasuredCurrent(i+1));

    if (changed) {
      xml.writeStartElement(QString("RohdeSchwarzNGE103BChannel"));
      xml.writeAttribute("id", QString::number(i+1));
      xml.writeAttribute("State", nge103BOutputState_[i]==true ? "1" : "0");
      xml.writeAttribute("Mode", QString::number(nge103BOutputMode_[i]));
      xml.writeAttribute("U", QString::number(nge103BVoltage_[i], 'f', 3));
      xml.writeAttribute("mU", QString::number(nge103BMeasuredVoltage_[i], 'f', 3));
      xml.writeAttribute("I", QString::number(nge103BCurrent_[i], 'f', 3));
      xml.writeAttribute("mI", QString::number(nge103BMeasuredCurrent_[i], 'f', 3));
      xml.writeEndElement();
    }
  }
  xml.writeEndElement();

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}

void Thermo2DAQModel::keithleyInfoChanged()
{
  NQLogDebug("Thermo2DAQModel") << "keithleyInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  xml.writeStartElement("KeithleyDAQ6510");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));

  for (unsigned int card=0;card<2;++card) {
    for (unsigned int channel=0;channel<10;++channel) {
      unsigned int sensor = (card+1)*100 + channel + 1;

      bool changed = false;
      changed |= updateIfChanged<bool>(keithleyState_[card][channel], keithleyModel_->getSensorState(sensor)==READY ? "1" : "0");
      changed |= updateIfChanged<float>(keithleyTemperature_[card][channel], keithleyModel_->getTemperature(sensor));

      if (changed) {
        xml.writeStartElement(QString("KeithleyDAQ6510Sensor"));
        xml.writeAttribute("id", QString::number(sensor));
        xml.writeAttribute("State", keithleyState_[card][channel]==true ? "1" : "0");
        xml.writeAttribute("T", QString::number(keithleyTemperature_[card][channel], 'f', 4));
        xml.writeEndElement();
      }
    }
  }
  xml.writeEndElement();

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}

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

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <TDatime.h>

#include "ThermoDAQ2StreamReader.h"

ThermoDAQ2StreamReader::ThermoDAQ2StreamReader(QStringList arguments,
                                               QObject* parent)
  : QObject(parent),
    arguments_(arguments),
    measurementValid_(false)
{
  log_.message = "";
}

void ThermoDAQ2StreamReader::run()
{
  process();
  emit finished();
}

void ThermoDAQ2StreamReader::processRohdeSchwarzNGE103B(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);
}

void ThermoDAQ2StreamReader::processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml)
{
  int id = xml.attributes().value("id").toString().toInt();
  float mU = xml.attributes().value("mU").toString().toFloat();
  float mI = xml.attributes().value("mI").toString().toFloat();

  measurement_.nge103BVoltage[id-1] = mU;
  measurement_.nge103BCurrent[id-1] = mI;
}

void ThermoDAQ2StreamReader::processLog(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::TextDate);
  log_.uTime = dt.toTime_t();
  log_.datime = TDatime(measurement_.uTime);

  log_.message = xml.readElementText(QXmlStreamReader::IncludeChildElements).toStdString();

  ologtree_->Fill();
}

void ThermoDAQ2StreamReader::processDAQStarted(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

  measurementValid_ = true;
}

void ThermoDAQ2StreamReader::processLine(QString& line)
{
  QXmlStreamReader xml(line);
  while (!xml.atEnd()) {
    xml.readNextStartElement();
    if (xml.isStartElement()) {
      if (xml.name()=="RohdeSchwarzNGE103B") {
        processRohdeSchwarzNGE103B(xml);
      }
      if (xml.name()=="RohdeSchwarzNGE103BChannel") {
        processRohdeSchwarzNGE103BChannel(xml);
      }
      if (xml.name()=="Log") {
        processLog(xml);
      }
      if (xml.name()=="DAQStarted") {
        processDAQStarted(xml);
      }
    }
  }

  if (measurementValid_) otree_->Fill();
}

void ThermoDAQ2StreamReader::process()
{
  if (arguments_.size()<2 || arguments_.size()>3) return;

  QFile file(arguments_.last());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  QFileInfo fi(file);
  QString base = fi.baseName();
  std::string rootname = base.toStdString();
  rootname += ".root";

  ofile_ = new TFile(rootname.c_str(), "RECREATE");

  otree_ = new TTree("thermoDAQ2", "thermoDAQ2");

  otree_->Branch("uTime", &measurement_.uTime, "uTime/i");
  otree_->Branch("datime", &measurement_.datime, 1024, 2);

  for (int i=0;i<3;++i) {
    char dummy1[20];
    char dummy2[20];

    sprintf(dummy1, "nge103BVoltage_%d", i+1);
    sprintf(dummy2, "nge103BVoltage_%d/I", i+1);
    otree_->Branch(dummy1, &measurement_.nge103BVoltage[i], dummy2);

    sprintf(dummy1, "nge103BCurrent_%d", i+1);
    sprintf(dummy2, "nge103BCurrent_%d/F", i+1);
    otree_->Branch(dummy1, &measurement_.nge103BCurrent[i], dummy2);
  }

  ologtree_ = new TTree("thermoLog", "thermoLog");
  ologtree_->Branch("uTime", &log_.uTime, "uTime/i");
  ologtree_->Branch("datime", &log_.datime, 1024, 2);
  ologtree_->Branch("message", &log_.message);

  QString line;
  QTextStream in(&file);
  while (!in.atEnd()) {
    line = in.readLine();
    processLine(line);
  }

  ofile_->Write();
  delete ofile_;
}

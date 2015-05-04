#include <iostream>

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <TDatime.h>

#include "DefoDAQStreamReader.h"

DefoDAQStreamReader::DefoDAQStreamReader(QStringList arguments,
                                         QObject* parent)
: QObject(parent),
  arguments_(arguments),
  measurementValid_(false)
{

}

void DefoDAQStreamReader::run()
{
  process();
  emit finished();
}

void DefoDAQStreamReader::processDAQStarted(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

  measurementValid_ = true;
}

void DefoDAQStreamReader::processJulaboTemperature(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

  measurement_.chillerState = xml.attributes().value("state").toString().toInt();
  measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
  measurement_.workingTemperature = xml.attributes().value("work").toString().toFloat();
  measurement_.circulator = xml.attributes().value("circulator").toString().toInt();
}

void DefoDAQStreamReader::processKeithleyState(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

  int idx = xml.attributes().value("sensor").toString().toInt();
  measurement_.channelActive[idx] = xml.attributes().value("state").toString().toInt();
}

void DefoDAQStreamReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

  int idx = xml.attributes().value("sensor").toString().toInt();
  measurement_.temperature[idx] = xml.attributes().value("temperature").toString().toFloat();
}

void DefoDAQStreamReader::processConradState(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

  int idx = xml.attributes().value("switch").toString().toInt();
  measurement_.conradState[idx] = xml.attributes().value("state").toString().toInt();
}

void DefoDAQStreamReader::processLine(QString& line)
{
  QXmlStreamReader xml(line);
  while (!xml.atEnd()) {
    xml.readNextStartElement();
    if (xml.isStartElement()) {
      if (xml.name()=="JulaboTemperature") {
        processJulaboTemperature(xml);
      }
      if (xml.name()=="KeithleyState") {
        processKeithleyState(xml);
      }
      if (xml.name()=="KeithleyTemperature") {
        processKeithleyTemperature(xml);
      }
      if (xml.name()=="ConradSwitchState") {
        processConradState(xml);
      }
      if (xml.name()=="DAQStarted") {
        processDAQStarted(xml);
      }
    }
  }

  if (measurementValid_) otree_->Fill();
}

void DefoDAQStreamReader::process()
{
  if (arguments_.size()!=2) return;

  QFile file(arguments_.at(1));
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  QFileInfo fi(file);
  QString base = fi.baseName();
  std::string rootname = base.toStdString();
  rootname += ".root";

  ofile_ = new TFile(rootname.c_str(), "RECREATE");

  otree_ = new TTree("thermoDAQ", "thermoDAQ");

  otree_->Branch("uTime", &measurement_.uTime, "uTime/i");
  otree_->Branch("datime", &measurement_.datime, 1024, 2);

  otree_->Branch("chillerState", &measurement_.chillerState, "chillerState/I");
  otree_->Branch("bathTemperature", &measurement_.bathTemperature, "bathTemperature/F");
  otree_->Branch("workingTemperature", &measurement_.workingTemperature, "workingTemperature/F");
  otree_->Branch("circulator", &measurement_.circulator, "circulator/I");

  for (int i=0;i<10;++i) {
    char dummy1[20];
    char dummy2[20];

    sprintf(dummy1, "channelActive%d", i);
    sprintf(dummy2, "channelActive%d/I", i);
    otree_->Branch(dummy1, &measurement_.channelActive[i], dummy2);

    sprintf(dummy1, "temperature%d", i);
    sprintf(dummy2, "temperature%d/F", i);
    otree_->Branch(dummy1, &measurement_.temperature[i], dummy2);
  }

  for (int i=0;i<10;++i) {
    char dummy1[20];
    char dummy2[20];

    sprintf(dummy1, "conradState%d", i);
    sprintf(dummy2, "conradState%d/I", i);
    otree_->Branch(dummy1, &measurement_.conradState[i], dummy2);
  }

  QString line;
  QTextStream in(&file);
  while (!in.atEnd()) {
    line = in.readLine();
    processLine(line);
  }

  ofile_->Write();
  delete ofile_;
}

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

#include <iostream>

#include <QtCore>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDirIterator>

#include <TDatime.h>

#include "Analyser.h"

DataFileDate::DataFileDate()
: date_(QDate()),
  n_(0)
{

}

DataFileDate::DataFileDate(const DataFileDate& other)
: date_(other.getDate().year(), other.getDate().month(), other.getDate().day()),
  n_(other.getNumber())
{

}

DataFileDate::DataFileDate(const QDate& date, int n)
: date_(date),
  n_(n)
{

}

DataFileDate::DataFileDate(int y, int m, int d, int n)
: date_(y, m, d),
  n_(n)
{

}

Analyser::Analyser(QStringList& arguments)
: arguments_(arguments),
  measurementValid_(false)
{
  dataValid_ = false;
  for (int i=0;i<5;++i) switchStateValid_[i] = false;
  for (int i=0;i<3;++i) gaugeValid_[i] = false;
}

void Analyser::analyse()
{
  bool hasStartDate = false;
  QDate startDate;
  if (arguments_.size()>=3) {
    QStringList list = arguments_[2].split('-', Qt::SkipEmptyParts);
    if (list.size()!=3) {
      std::cout << "cannot parse start date " << arguments_[2].toStdString() << std::endl;
      QCoreApplication::quit();
      return;
    }

    int y = list[0].toInt();

    int m = list[1].toInt();
    if (m>12) return;

    int d = list[2].toInt();

    startDate = QDate(y, m, d);
    while (!startDate.isValid() && d>1) {
      d--;
      startDate = QDate(y, m, d);
    }

    hasStartDate = true;
  }

  bool hasEndDate = false;
  QDate endDate;
  if (arguments_.size()==4) {
    QStringList list = arguments_[3].split('-', Qt::SkipEmptyParts);
    if (list.size()!=3) {
      std::cout << "cannot parse end date " << arguments_[3].toStdString() << std::endl;
      QCoreApplication::quit();
      return;
    }

    int y = list[0].toInt();

    int m = list[1].toInt();
    if (m>12) return;

    int d = list[2].toInt();

    endDate = QDate(y, m, d);
    while (!endDate.isValid() && d>1) {
      d--;
      endDate = QDate(y, m, d);
    }

    hasEndDate = true;
  }

  QDirIterator it(arguments_[1], QDirIterator::NoIteratorFlags);
  while (it.hasNext()) {
    QString filename = it.next();

    if (!filename.endsWith(".xml")) continue;
    if (!filename.contains("pumpstation_")) continue;

    QString dateStamp = filename;

    dateStamp.remove(".xml");
    dateStamp.remove(0, dateStamp.indexOf("pumpstation_"));
    dateStamp.remove("pumpstation_");

    dateStamp.insert(6, "_");
    dateStamp.insert(4, "_");

    QStringList list = dateStamp.split('_', Qt::SkipEmptyParts);
    if (list.size()!=4) continue;

    int y = list[0].toInt();
    int m = list[1].toInt();
    int d = list[2].toInt();
    int n = list[3].toInt();

    QDate date(y, m, d);

    if (hasStartDate && date<startDate) continue;
    if (hasEndDate && date>endDate) continue;

    fileMap_[DataFileDate(date, n)] = filename;
  }

  QString filename = arguments_[0];
  if (!filename.endsWith(".root")) filename += ".root";
  std::string rootname = filename.toStdString();

  ofile_ = new TFile(rootname.c_str(), "RECREATE");

  otree_ = new TTree("pumpstation", "pumpstation");

  otree_->Branch("uTime", &measurement_.uTime, "uTime/i");
  otree_->Branch("datime", &measurement_.datime, 1024, 2);

  for (int i=0;i<5;++i) {
    char dummy1[20];
    char dummy2[20];

    sprintf(dummy1, "switchState%d", i);
    sprintf(dummy2, "switchState%d/I", i);
    otree_->Branch(dummy1, &measurement_.switchState[i], dummy2);
  }

  for (int i=0;i<3;++i) {
    char dummy1[20];
    char dummy2[20];

    sprintf(dummy1, "gaugeState%d", i);
    sprintf(dummy2, "gaugeState%d/I", i);
    otree_->Branch(dummy1, &measurement_.gaugeState[i], dummy2);

    sprintf(dummy1, "pressure%d", i);
    sprintf(dummy2, "pressure%d/D", i);
    otree_->Branch(dummy1, &measurement_.pressure[i], dummy2);
  }

  for (std::map<DataFileDate, QString>::iterator it = fileMap_.begin();
      it!=fileMap_.end();
      ++it) {
    processFile(it->second);
  }

  ofile_->Write();
  delete ofile_;

  QCoreApplication::quit();
}

void Analyser::processFile(const QString& filename)
{
  std::cerr << "processing file: " << filename.toLocal8Bit().constData() << std::endl;

  QFile file(filename);
  if (file.open(QFile::ReadOnly)) {
    QXmlStreamReader reader(&file);

    while (!reader.atEnd()) {

      QXmlStreamReader::TokenType tokentype = reader.readNext();

      if (tokentype==QXmlStreamReader::EndElement) {

        if (reader.name()=="Status") {
          if (dataValid_) {
            dumpData();
          }
        }

      }

      if (tokentype==QXmlStreamReader::StartElement) {

        QXmlStreamAttributes attributes = reader.attributes();

        if (attributes.hasAttribute("time")) {
          utime_ = QDateTime::fromString(attributes.value("time").toString(), Qt::ISODate);
        }

        if (reader.name()=="ConradSwitch") {

          if (attributes.hasAttribute("id") &&
              attributes.hasAttribute("state")) {
            int id = attributes.value("id").toString().toInt();
            bool state = attributes.value("state").toString().toInt();

            switchState_[id] = state;
            switchStateValid_[id] = true;

            if (attributes.hasAttribute("time")) {
              dumpData();
            }
          }
        }

        if (reader.name()=="LeyboldGraphixThree") {
          if (attributes.hasAttribute("id") &&
              attributes.hasAttribute("status") &&
              attributes.hasAttribute("p")) {
            int id = attributes.value("id").toString().toInt() - 1;
            bool state = attributes.value("status").toString().toInt();
            float p = attributes.value("p").toString().toFloat();

            gaugeState_[id] = state;
            gaugeValid_[id] = true;
            gaugePressure_[id] = p;

            if (attributes.hasAttribute("time")) {
              dumpData();
            }
          }
        }

        if (dataValid_==false) {
          bool valid = true;
          for (int i=0;i<5;++i) {
            valid &= switchStateValid_[i];
            //std::cout << switchStateValid_[i] << " ";
          }
          for (int i=0;i<3;++i) {
            valid &= gaugeValid_[i];
            //std::cout << gaugeValid_[i] << " ";
          }
          //std::cout << std::endl;

          dataValid_ = valid;
        }
      }
    }
  }
}

void Analyser::dumpData()
{
  // std::cout << utime_.toTime_t();

  measurement_.uTime = utime_.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

  for (int i=0;i<5;++i) {
    // std::cout << " " << switchState_[i];

    measurement_.switchState[i] = switchState_[i];
  }
  for (int i=0;i<3;++i) {
    // std::cout << " " << gaugeState_[i] << " " << std::scientific << gaugePressure_[i];

    measurement_.gaugeState[i] = gaugeState_[i];
    measurement_.pressure[i] = gaugePressure_[i];
  }

  // std::cout << std::endl;

  otree_->Fill();
}

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

#include <QDateTime>
#include <QMap>
#include <QVariant>
#include <QFile>

#include <nqlogger.h>

#include "ApplicationConfig.h"
#include "ThermoDAQWebInfo.h"
#include "ThermoDAQModel.h"

ThermoDAQWebInfo::ThermoDAQWebInfo(ThermoDAQModel* model, QObject* parent) :
    QObject(parent),
    model_(model)
{
  lastUpdate_ = QDateTime::currentDateTime().toString(Qt::ISODate);

  htmlSource_ = "";
  QString filename(Config::CMSTkModLabBasePath.c_str());
  QFile file(filename + "/share/thermoDAQ_webinfo.html");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    htmlSource_ = stream.readAll();
  }

  ApplicationConfig* config = ApplicationConfig::instance();
  htmlFile_ = config->getValue<std::string>("HTMLFile").c_str();

  NQLog("ThermoDAQWebInfo") << "writing to file " << htmlFile_;

  connect(model_, SIGNAL(daqStateChanged(bool)),
	  this, SLOT(daqStateChanged(bool)));
  connect(&executionTimer_, SIGNAL(timeout()), 
	  this, SLOT(updateWebInfo()));
  
  executionTimer_.start(10000);
}

void ThermoDAQWebInfo::updateWebInfo()
{
  NQLog("ThermoDAQWebInfo", NQLog::Spam) << "updateWebInfo";

  const Measurement_t& m = model_->getMeasurement();

  QString htmlTemp = htmlSource_;

  htmlTemp.replace("@DATETIME@", m.dt.toString(Qt::ISODate));

  htmlTemp.replace("@DAQSTATE@", QString::number((int)m.daqState));

  htmlTemp.replace("@HUBERCIRCULATOR@", QString::number(m.circulator));
  htmlTemp.replace("@HUBERWORKINGTEMPERATURE@", QString::number(m.workingTemperature, 'f', 2));
  htmlTemp.replace("@HUBERBATHTEMPERATURE@", QString::number(m.bathTemperature, 'f', 2));

  for (int i=0;i<10;++i) {
    htmlTemp.replace(QString("@KEITHLEYSENSORSTATE%1@").arg(i), QString::number(m.channelActive[i]));
    htmlTemp.replace(QString("@KEITHLEYTEMPERATURE%1@").arg(i), QString::number(m.temperature[i], 'f', 2));
  }

  // htmlTemp.replace("@@", m.);

  QFile file(htmlFile_);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << htmlTemp;
  }
}

void ThermoDAQWebInfo::daqStateChanged(bool state)
{
  updateWebInfo();
}

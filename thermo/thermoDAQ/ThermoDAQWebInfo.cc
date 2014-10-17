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

  connect(model_, SIGNAL(daqStateChanged(bool)),
	  this, SLOT(daqStateChanged(bool)));
  connect(&executionTimer_, SIGNAL(timeout()), 
	  this, SLOT(updateWebInfo()));
  
  executionTimer_.start(10000);
}

void ThermoDAQWebInfo::updateWebInfo()
{
  NQLog("ThermoDAQWebInfo") << "updateWebInfo";

  const Measurement_t& m = model_->getMeasurement();

  QString htmlTemp = htmlSource_;

  htmlTemp.replace("@DATETIME@", m.dt.toString(Qt::ISODate));
  
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

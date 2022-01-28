/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QFile>
#include <QTextStream>
#include <QPlainTextDocumentLayout>
#include <QFont>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include "Thermo2ScriptModel.h"

Thermo2ScriptModel::Thermo2ScriptModel(Thermo2DAQModel* daqModel,
				       LeyboldGraphixOneModel* leyboldModel,
				       HuberUnistat525wModel* huberModel,
				       MartaModel* martaModel,
				       RohdeSchwarzNGE103BModel* nge103BModel,
				       KeithleyDAQ6510Model* keithleyModel,
				       Thermo2ThroughPlaneModel* t2tpModel,
				       QObject *parent)
: QObject(parent),
  daqModel_(daqModel),
  leyboldModel_(leyboldModel),
  huberModel_(huberModel),
  martaModel_(martaModel),
  nge103BModel_(nge103BModel),
  keithleyModel_(keithleyModel),
  t2tpModel_(t2tpModel)
{
  script_ = new QTextDocument(this);
  script_->setDocumentLayout(new QPlainTextDocumentLayout(script_));
  script_->setDefaultFont(QFont("Courier New",10));

  currentScriptFilename_ = QString();

  scriptThread_ = new Thermo2ScriptThread(this,
					  leyboldModel_,
                                          huberModel_,
                                          martaModel_,
                                          nge103BModel_,
                                          keithleyModel_,
                                          t2tpModel_,
                                          this);
  connect(scriptThread_, SIGNAL(started()), this, SLOT(executionStarted()));
  connect(scriptThread_, SIGNAL(finished()), this, SLOT(executionFinished()));

  if (leyboldModel_) {
    connect(leyboldModel_, SIGNAL(message(const QString &)),
	    this, SLOT(doAppendMessageText(const QString &)));
    connect(leyboldModel_, SIGNAL(log(const QString &)),
	    this, SLOT(log(const QString &)));
  }

  if (huberModel_) {
    connect(huberModel_, SIGNAL(message(const QString &)),
	    this, SLOT(doAppendMessageText(const QString &)));
    connect(huberModel_, SIGNAL(log(const QString &)),
	    this, SLOT(log(const QString &)));
  }

  if (martaModel_) {
    connect(martaModel_, SIGNAL(message(const QString &)),
	    this, SLOT(doAppendMessageText(const QString &)));
    connect(martaModel_, SIGNAL(log(const QString &)),
	    this, SLOT(log(const QString &)));
  }

  connect(nge103BModel_, SIGNAL(message(const QString &)),
	  this, SLOT(doAppendMessageText(const QString &)));
  connect(nge103BModel_, SIGNAL(log(const QString &)),
	  this, SLOT(log(const QString &)));

  connect(keithleyModel_, SIGNAL(message(const QString &)),
	  this, SLOT(doAppendMessageText(const QString &)));
  connect(keithleyModel_, SIGNAL(log(const QString &)),
	  this, SLOT(log(const QString &)));

  if (t2tpModel_) {
    connect(t2tpModel_, SIGNAL(message(const QString &)),
            this, SLOT(doAppendMessageText(const QString &)));
    connect(t2tpModel_, SIGNAL(log(const QString &)),
            this, SLOT(log(const QString &)));
  }

  connect(&executionTimer_, SIGNAL(timeout()), this, SLOT(executionHeartBeat()));
}

void Thermo2ScriptModel::openScript(const QString filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  script_->setPlainText(file.readAll());

  currentScriptFilename_ = filename;
}

void Thermo2ScriptModel::saveScript(const QString filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QTextStream out(&file);
  out << script_->toPlainText();

  currentScriptFilename_ = filename;
}

void Thermo2ScriptModel::executeScript()
{
  scriptThread_->executeScript(script_->toPlainText());
}

void Thermo2ScriptModel::abortScript()
{
  scriptThread_->abortScript();
}

void Thermo2ScriptModel::executionStarted()
{
  NQLogMessage("Thermo2ScriptModel") << "executionStarted()";

  emit setControlsEnabled(false);
  emit clearMessageText();

  QDateTime& utime = daqModel_->currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);

  xml.writeStartElement("Log");
  xml.writeAttribute("time", utime.toString());
  xml.writeCharacters("script execution started");
  xml.writeEndElement();

  daqModel_->customDAQMessage(buffer);

  executionTime_ = 0;
  executionTimer_.start(1000);
}

void Thermo2ScriptModel::executionFinished()
{
  NQLogMessage("Thermo2ScriptModel") << "executionFinished()";

  executionTimer_.stop();

  QDateTime& utime = daqModel_->currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);

  xml.writeStartElement("Log");
  xml.writeAttribute("time", utime.toString());
  xml.writeCharacters("script execution finished");
  xml.writeEndElement();

  daqModel_->customDAQMessage(buffer);

  emit setControlsEnabled(true);
}

void Thermo2ScriptModel::startMeasurement()
{
  if (daqModel_->daqState()==false) {
    this->message("start Measurement");
    daqModel_->startMeasurement();
  }
}

void Thermo2ScriptModel::stopMeasurement()
{
  if (daqModel_->daqState()==true) {
    this->message("stop Measurement");
    daqModel_->stopMeasurement();
  }
}

void Thermo2ScriptModel::message(int value)
{
  QString message = QString("%1").arg(value);
  this->doAppendMessageText(message);
}

void Thermo2ScriptModel::message(uint value)
{
  QString message = QString("%1").arg(value);
  this->doAppendMessageText(message);
}

void Thermo2ScriptModel::message(double value)
{
  QString message = QString("%1").arg(value);
  this->doAppendMessageText(message);
}

void Thermo2ScriptModel::message(const QString & text)
{
  QString message = QString("%1").arg(text);
  this->doAppendMessageText(message);
}

void Thermo2ScriptModel::log(const QString & text)
{
  QDateTime& utime = daqModel_->currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);

  xml.writeStartElement("Log");
  xml.writeAttribute("time", utime.toString());
  xml.writeCharacters(text);
  xml.writeEndElement();

  daqModel_->customDAQMessage(buffer);
}

void Thermo2ScriptModel::doClearMessageText()
{
  emit clearMessageText();
}

void Thermo2ScriptModel::doAppendMessageText(const QString & text)
{
  if (!executionTimer_.isActive()) return;

  static QString TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
  QDateTime dt = QDateTime::currentDateTime();
  QString message = QString("%1 - %2")
                .arg(dt.toString(TIME_FORMAT))
                .arg(text);
  emit appendMessageText(message);
}

void Thermo2ScriptModel::executionHeartBeat()
{
  executionTime_++;
}

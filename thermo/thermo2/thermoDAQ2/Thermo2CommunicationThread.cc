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

#include <iostream>

#include <QDataStream>

#include <nqlogger.h>

#include "Thermo2CommunicationThread.h"

Thermo2CommunicationThread::Thermo2CommunicationThread(Thermo2DAQModel* daqModel,
    HuberUnistat525wModel* huberModel,
    MartaModel* martaModel,
    AgilentTwisTorr304Model* agilentModel,
    LeyboldGraphixOneModel* leyboldModel,
    RohdeSchwarzNGE103BModel* nge103BModel,
    KeithleyDAQ6510Model* keithleyModel,
    qintptr socketDescriptor, QObject *parent)
: QThread(parent),
  daqModel_(daqModel),
  huberModel_(huberModel),
  martaModel_(martaModel),
  agilentModel_(agilentModel),
  leyboldModel_(leyboldModel),
  nge103BModel_(nge103BModel),
  keithleyModel_(keithleyModel),
  socketDescriptor_(socketDescriptor)
{
  NQLogDebug("Thermo2CommunicationThread") << "Thermo2CommunicationThread()";
  
  socket_ = new QTcpSocket(this);
  
  connect(socket_, SIGNAL(readyRead()),
    this, SLOT(readData()));
  connect(socket_, SIGNAL(disconnected()),
    socket_, SLOT(deleteLater()));
  
  socket_->setSocketDescriptor(socketDescriptor_);
}

Thermo2CommunicationThread::~Thermo2CommunicationThread()
{
  NQLogDebug("Thermo2CommunicationThread") << "~CommunicationThread()";
}

void Thermo2CommunicationThread::run()
{
  NQLogDebug("Thermo2CommunicationThread") << "run()";

  exec();
}

void Thermo2CommunicationThread::readData()
{
  QTextStream os(socket_);
  os.setAutoDetectUnicode(true);
  
  QString line;
  if (socket_->canReadLine()) {
    line = socket_->readLine();

    QStringList tokens = line.split(QRegExp("[ \r\n][ \r\n]*"));
    if (tokens.last().isEmpty()) tokens.takeLast();

    if (tokens[0] == "GET") {
      
      NQLogDebug("Thermo2CommunicationThread::readData()") << "GET " << tokens[1].toStdString();
      
      os << "HTTP/1.0 200 Ok\r\n"
      "Content-Type: text/html; charset=\"utf-8\"\r\n"
      "\r\n"
      "<h1>Nothing to see here</h1>\n";

    } else if (tokens[0] == "COM") {

      bool ret = handleCommand(tokens, os);
      if (ret) {
        os << "OK\r\n";
      } else {
        os << "ERR\r\n";
      }

    } else {
      os << "ERR\r\n";
    }
  }
  
  socket_->close();
  
  socket_->disconnectFromHost();
  socket_->waitForDisconnected();
  
  quit();
}

bool Thermo2CommunicationThread::handleCommand(QStringList& tokens, QTextStream& os)
{
  QMutexLocker locker(&mutex_);

  QStringList pars;

  int idx = 0;
  QString buffer;
  for (auto t : tokens) {
    buffer = buffer + "|" + t + "| ";
    if (idx>1) pars << t;
    idx++;
  }
  NQLogDebug("Thermo2CommunicationThread::handleCommand()") << buffer.toStdString();

  if (tokens[1] == "start") {
    if (pars.count()!=0) return false;
    daqModel_->startMeasurement();

  } else if (tokens[1] == "stop") {
    if (pars.count()!=0) return false;
    daqModel_->stopMeasurement();

  } else if (tokens[1] == "getKp") {
    if (pars.count()!=0) return false;
    os << QString::number(huberModel_->getKp()) << " ";

  } else if (tokens[1] == "setKp") {
    if (pars.count()!=1) return false;
    huberModel_->setKp(pars[0].toInt());

  } else if (tokens[1] == "getTn") {
    if (pars.count()!=0) return false;
    os << QString::number(huberModel_->getTn(), 'f', 1) << " ";

  } else if (tokens[1] == "setTn") {
    if (pars.count()!=1) return false;
    huberModel_->setTn(pars[0].toFloat());

  } else if (tokens[1] == "getTv") {
    if (pars.count()!=0) return false;
    os << QString::number(huberModel_->getTv(), 'f', 1) << " ";

  } else if (tokens[1] == "setTv") {
    if (pars.count()!=1) return false;
    huberModel_->setTv(pars[0].toFloat());

  } else if (tokens[1] == "setPID") {
    if (pars.count()!=3) return false;
    huberModel_->setPID(pars[0].toInt(), pars[1].toFloat(), pars[2].toFloat());

  } else if (tokens[1] == "setSetPoint") {
    if (pars.count()!=1) return false;
    huberModel_->setTemperatureSetPoint(pars[0].toFloat());

  } else if (tokens[1] == "setTemperatureControl") {
    if (pars.count()!=1) return false;
    huberModel_->setTemperatureControlEnabled(pars[0].toInt());

  } else if (tokens[1] == "setOutputState") {
    if (pars.count()!=2) return false;
    nge103BModel_->setOutputState(pars[0].toInt(), pars[1].toInt());

  } else if (tokens[1] == "setVoltage") {
    if (pars.count()!=2) return false;
    nge103BModel_->setVoltage(pars[0].toInt(), pars[1].toFloat());

  } else if (tokens[1] == "setCurrent") {
    if (pars.count()!=2) return false;
    nge103BModel_->setCurrent(pars[0].toInt(), pars[1].toFloat());

  }

  return true;
}

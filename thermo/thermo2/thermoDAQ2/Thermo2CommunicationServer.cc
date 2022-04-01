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
#include <QTcpSocket>
#include <QHostInfo>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "Thermo2CommunicationThread.h"

#include "Thermo2CommunicationServer.h"

Thermo2CommunicationServer::Thermo2CommunicationServer(Thermo2DAQModel* daqModel,
    HuberUnistat525wModel* huberModel,
    MartaModel* martaModel,
    AgilentTwisTorr304Model* agilentModel,
    LeyboldGraphixOneModel* leyboldModel,
    RohdeSchwarzNGE103BModel* nge103BModel,
    KeithleyDAQ6510Model* keithleyModel,
    QObject *parent)
 : QTcpServer(parent),
   daqModel_(daqModel),
   huberModel_(huberModel),
   martaModel_(martaModel),
   agilentModel_(agilentModel),
   leyboldModel_(leyboldModel),
   nge103BModel_(nge103BModel),
   keithleyModel_(keithleyModel)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  QString ipAddress = ApplicationConfig::instance()->getValue<std::string>("main", "CommServerIP").c_str();
  if (ipAddress.isEmpty()) {
    QHostInfo hostinfo = QHostInfo::fromName(ApplicationConfig::instance()->getValue<std::string>("main", "CommServerHostname").c_str());
    if (!hostinfo.addresses().isEmpty()) {
      QHostAddress address = hostinfo.addresses().first();
      // use the first IP address

      ipAddress = address.toString();
    } else {
      ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
  }
  
  quint16 port = config->getValue<unsigned int>("main", "CommServerPort");
  
  if (!listen(QHostAddress(ipAddress), port)) {
    NQLogMessage("Thermo2CommunicationServer") << "Unable to start the server: " << errorString().toStdString();
    return;
  }
  
  NQLogDebug("Thermo2CommunicationServer") << "server listening on "
    << serverAddress().toString().toStdString()
    << ":"
    << serverPort();
}

void Thermo2CommunicationServer::incomingConnection(qintptr socketDescriptor)
{
  NQLogDebug("Thermo2CommunicationServer") << "incomingConnection(qintptr socketDescriptor)";

  Thermo2CommunicationThread* commthread = new Thermo2CommunicationThread(daqModel_,
      huberModel_,
      martaModel_,
      agilentModel_,
      leyboldModel_,
      nge103BModel_,
      keithleyModel_,
      socketDescriptor, this);
  connect(commthread, &Thermo2CommunicationThread::finished,
          commthread, &Thermo2CommunicationThread::deleteLater);
  commthread->start();
}

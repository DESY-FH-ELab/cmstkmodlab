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

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "Thermo2CommunicationThread.h"

#include "Thermo2CommunicationServer.h"

Thermo2CommunicationServer::Thermo2CommunicationServer(QObject *parent)
 : QTcpServer(parent)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  QString ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
  quint16 port = config->getValue<unsigned int>("CommServerPort", 56666);
  
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

  Thermo2CommunicationThread* commthread = new Thermo2CommunicationThread(socketDescriptor, this);
  connect(commthread, &Thermo2CommunicationThread::finished,
          commthread, &Thermo2CommunicationThread::deleteLater);
  commthread->start();
}

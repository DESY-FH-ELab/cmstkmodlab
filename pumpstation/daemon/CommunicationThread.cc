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
#include <QtNetwork>

#include <nqlogger.h>
#include "ApplicationConfig.h"

#include "CommunicationThread.h"

CommunicationThread::CommunicationThread(PumpStationModel* model,
                                         QObject *parent)
 : QThread(parent),
   model_(model)
{

}

void CommunicationThread::run()
{
  server_ = new CommunicationServer(model_);

  // find out which IP to connect to
  QString ipAddress;
  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
  // use the first non-localhost IPv4 address
  for (int i = 0; i < ipAddressesList.size(); ++i) {
    if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
        ipAddressesList.at(i).toIPv4Address()) {
      ipAddress = ipAddressesList.at(i).toString();
      break;
    }
  }
  // if we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty())
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

  quint16 port = ApplicationConfig::instance()->getValue("ServerPort", 63432);

  if (!server_->listen(QHostAddress(ipAddress), port)) {
    NQLog("pumpstation") << "Unable to start the server: " << server_->errorString().toStdString();
    return;
  }

  NQLog("pumpstation") << "server listening on "
                       << server_->serverAddress().toString().toStdString()
                       << ":"
                       << server_->serverPort();

  exec();
}

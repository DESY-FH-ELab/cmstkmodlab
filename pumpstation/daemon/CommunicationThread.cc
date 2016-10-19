#include <iostream>

#include <QtCore>
#include <QtNetwork>

#include <nqlogger.h>
#include "ApplicationConfig.h"

#include "CommunicationThread.h"

CommunicationThread::CommunicationThread(ConradModel* conradModel,
                                         QObject *parent)
 : QThread(parent),
   conradModel_(conradModel)
{

}

void CommunicationThread::run()
{
  server_ = new CommunicationServer(conradModel_);

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

  quint16 port = ApplicationConfig::instance()->getValue("SERVERPORT", 63432);

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

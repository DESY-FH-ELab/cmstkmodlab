/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
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
#include <ApplicationConfig.h>

#include "Controller.h"

Controller::Controller(QStringList& arguments)
 : arguments_(arguments)
{
  // find out which IP to connect to
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

  socket_ = new QTcpSocket(this);

  connect(socket_, SIGNAL(connected()), this, SLOT(sendCommand()));
  connect(socket_, SIGNAL(readyRead()), this, SLOT(readResponse()));
}

void Controller::connectToServer()
{
  NQLogDebug("Controller") << "void Controller::connectToServer()";

  quint16 port = ApplicationConfig::instance()->getValue<unsigned int>("CommServerPort", 56666);

  socket_->abort();
  socket_->connectToHost(ipAddress, port);

  if (!socket_->waitForConnected(500)) {
    NQLogDebug("Controller") << "The following error occurred: " << socket_->errorString().toStdString();

    std::cout << "ERR" << std::endl;

    socket_->close();
    socket_->deleteLater();
    QCoreApplication::quit();
  }
}

void Controller::sendCommand()
{
  NQLogDebug("Controller") << "void Controller::sendCommand()";

  QString command = "COM ";
  for (int i=0; i<arguments_.size(); ++i) {
    if (i>0) command += " ";
    command += arguments_.at(i);
  }

  NQLogDebug("Controller") << "command.length() = " << command.length();

  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << (quint16)command.length();
  out << command;

  socket_->write(block);
  socket_->flush();
}

void Controller::readResponse()
{
  NQLogDebug("Controller") << "void Controller::readResponse()";

  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_4_0);

  quint16 blockSize = 0;
  in >> blockSize;

  NQLogDebug("Controller") << "blockSize = " << blockSize;

  QString response;
  in >> response;

  NQLogDebug("Controller") << "response: (" << blockSize << ") |" << response.toStdString() << "|";

  std::cout << response.toStdString() << std::endl;

  socket_->close();

  QCoreApplication::quit();
}

void Controller::printHelp()
{
  std::cout <<
      "\n  Help\n"
      " ------\n\n"
      // " writeConfig                                      write server configuration file\n"
      // " setPumpState <pump (1/2)> <state (0/1)>          set the state of a pump\n"
      // " getPumpState <pump (1/2)>                        get the state of a pump\n"
      << std::endl;

  QCoreApplication::quit();
}

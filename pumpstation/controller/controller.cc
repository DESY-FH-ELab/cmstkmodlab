/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <iostream>

#include <QtCore>
#include <QtNetwork>

#include <nqlogger.h>
#include "ApplicationConfig.h"

#include "controller.h"

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
  connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(reportError(QAbstractSocket::SocketError)));

  connectToServer();
}

void Controller::connectToServer()
{
  NQLogDebug("controller") << "void Controller::connectToServer()";

  quint16 port = ApplicationConfig::instance()->getValue("SERVERPORT", 63432);

  socket_->abort();
  socket_->connectToHost(ipAddress, port);
}

void Controller::sendCommand()
{
  NQLogDebug("controller") << "void Controller::sendCommand()";

  QString command;
  for (int i=0; i<arguments_.size(); ++i) {
    if (i>0) command += " ";
    command += arguments_.at(i);
  }

  NQLogDebug("controller") << "command.length() = " << command.length();

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
  NQLogDebug("controller") << "void Controller::readResponse()";

  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_4_0);

  quint16 blockSize = 0;
  in >> blockSize;

  NQLogDebug("controller") << "blockSize = " << blockSize;

  QString response;
  in >> response;

  NQLogDebug("controller") << "response: (" << blockSize << ") |" << response.toStdString() << "|";

  std::cout << response.toStdString() << std::endl;

  socket_->close();

  QCoreApplication::quit();
}

void Controller::reportError(QAbstractSocket::SocketError socketError)
{
  switch (socketError) {
  case QAbstractSocket::RemoteHostClosedError:
    break;
  case QAbstractSocket::HostNotFoundError:
    NQLogFatal("controller") << "The host was not found. Please check the host name and port settings.";
    break;
  case QAbstractSocket::ConnectionRefusedError:
    NQLogFatal("controller") << "The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.";
    break;
  default:
    NQLogFatal("controller") << "The following error occurred: " << socket_->errorString().toStdString();
  }
}

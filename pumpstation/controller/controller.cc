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

#include "controller.h"

Controller::Controller(QObject *parent)
 : QObject(parent),
   networkSession(0)
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

  tcpSocket = new QTcpSocket(this);

  connect(tcpSocket, SIGNAL(connected()), this, SLOT(sendCommand()));
  connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
  connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(reportError(QAbstractSocket::SocketError)));

  QNetworkConfigurationManager manager;
  if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
    // Get saved network configuration
    QSettings settings(QSettings::UserScope, QLatin1String("TrollTech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
    settings.endGroup();

    // If the saved network configuration is not currently discovered use the system default
    QNetworkConfiguration config = manager.configurationFromIdentifier(id);
    if ((config.state() & QNetworkConfiguration::Discovered) !=
        QNetworkConfiguration::Discovered) {
      config = manager.defaultConfiguration();
    }

    networkSession = new QNetworkSession(config, this);
    connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));


    std::cout << "network session" << std::endl;

    networkSession->open();
  }

  requestNewFortune();
}

void Controller::requestNewFortune()
{
  blockSize = 0;
  tcpSocket->abort();
  tcpSocket->connectToHost(ipAddress, 63432);
}

void Controller::sendCommand()
{
  std::cout << "connected" << std::endl;

  QString command = "testCommand";

  std::cout << "command.length() = " << command.length() << std::endl;

  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << (quint16)command.length();
  out << command;

  tcpSocket->write(block);
  tcpSocket->flush();
}

void Controller::readFortune()
{
  QDataStream in(tcpSocket);
  in.setVersion(QDataStream::Qt_4_0);

  if (blockSize == 0) {
    if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
      return;

    in >> blockSize;
  }

  if (tcpSocket->bytesAvailable() < blockSize)
    return;

  QString nextFortune;
  in >> nextFortune;

  if (nextFortune == currentFortune) {
    QTimer::singleShot(0, this, SLOT(requestNewFortune()));
    return;
  }

  currentFortune = nextFortune;

  std::cout << currentFortune.toStdString() << std::endl;

  tcpSocket->close();

  QCoreApplication::quit();
}

void Controller::reportError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        std::cerr << "The host was not found. Please check the host name and port settings." << std::endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
      std::cerr << "The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct." << std::endl;
        break;
    default:
      std::cerr << "The following error occurred: " << tcpSocket->errorString().toStdString() << std::endl;
    }
}

void Controller::sessionOpened()
{
  // Save the used configuration
  QNetworkConfiguration config = networkSession->configuration();
  QString id;
  if (config.type() == QNetworkConfiguration::UserChoice)
    id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
  else
    id = config.identifier();

  QSettings settings(QSettings::UserScope, QLatin1String("TrollTech"));
  settings.beginGroup(QLatin1String("QtNetwork"));
  settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
  settings.endGroup();

  requestNewFortune();
}

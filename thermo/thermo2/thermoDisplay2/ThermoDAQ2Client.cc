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

#include <QtNetwork>

#include <nqlogger.h>

#include "ThermoDAQ2Client.h"

ThermoDAQ2Client::ThermoDAQ2Client(int socket, QObject *parent)
  : QObject(parent),
    socket_(socket)
{
  tcpSocket_ = new QTcpSocket(this);
  connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(read()));
}

void ThermoDAQ2Client::readDAQStatus()
{
  NQLogDebug("ThermoDAQ2Client") << "readDAQStatus()";

  blockSize_ = 0;
  tcpSocket_->connectToHost(QHostAddress::LocalHost, socket_);
  if (!tcpSocket_->waitForConnected(2000)) {
    tcpSocket_->abort();
    NQLogFatal("ThermoDAQClient") << "connection timeout";
  }
}

void ThermoDAQ2Client::read()
{
  NQLogDebug("ThermoDAQ2Client") << "read()";

  QByteArray in = tcpSocket_->readAll();
  QString buffer(in.data());

  tcpSocket_->abort();

  emit handleMessage(buffer);
}

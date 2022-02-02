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

Thermo2CommunicationThread::Thermo2CommunicationThread(qintptr socketDescriptor, QObject *parent)
: QThread(parent),
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
  NQLogDebug("Thermo2CommunicationThread") << "readData()";
  
  QTextStream os(socket_);
  os.setAutoDetectUnicode(true);
  
  QString line;
  // if (socket_->canReadLine()) {
    line = socket_->readLine();
    QStringList tokens = line.split(QRegExp("[ \r\n][ \r\n]*"));
    if (tokens[0] == "GET") {
      
      NQLogDebug("Thermo2CommunicationThread") << "GET " << tokens[1].toStdString();
      
      os << "HTTP/1.0 200 Ok\r\n"
      "Content-Type: text/html; charset=\"utf-8\"\r\n"
      "\r\n"
      "<h1>Nothing to see here</h1>\n";
    } else {
       NQLogDebug("Thermo2CommunicationThread") << line.toStdString();

       os << "blah";
    }
  //}
  
  socket_->close();
  
  socket_->disconnectFromHost();
  socket_->waitForDisconnected();
  
  quit();
}

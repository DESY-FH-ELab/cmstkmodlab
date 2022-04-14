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

#include "Thermo2DAQServer.h"

Thermo2DAQServer::Thermo2DAQServer(Thermo2DAQModel* model, QObject *parent)
  : QTcpServer(parent),
    model_(model)
{

}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
void Thermo2DAQServer::incomingConnection(int socketDescriptor)
{
  NQLogDebug("ThermoDAQServer") << "incomingConnection(int socketDescriptor)";

  Thermo2DAQServerThread *thread = new Thermo2DAQServerThread(socketDescriptor, model_, this);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  thread->start();
}
#else
void Thermo2DAQServer::incomingConnection(qintptr socketDescriptor)
{
  NQLogDebug("ThermoDAQServer") << "incomingConnection(qintptr socketDescriptor)";

  Thermo2DAQServerThread *thread = new Thermo2DAQServerThread(socketDescriptor, model_, this);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  thread->start();
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Thermo2DAQServerThread::Thermo2DAQServerThread(int socketDescriptor,
                                               Thermo2DAQModel* model,
                                               QObject *parent)
  : QThread(parent),
    socketDescriptor_(socketDescriptor),
    model_(model)
{

}
#else
Thermo2DAQServerThread::Thermo2DAQServerThread(qintptr socketDescriptor,
                                               Thermo2DAQModel* model,
                                               QObject *parent)
  : QThread(parent),
    socketDescriptor_(socketDescriptor),
    model_(model)
{

}
#endif

void Thermo2DAQServerThread::run()
{
  NQLogDebug("Thermo2DAQServerThread") << "run()";

  QTcpSocket tcpSocket;
  if (!tcpSocket.setSocketDescriptor(socketDescriptor_)) {
    NQLogDebug("Thermo2DAQServerThread") << "error setting socket descriptor";
    emit error(tcpSocket.error());
    return;
  }

  QString buffer;
  model_->createDAQStatusMessage(buffer);

  //QByteArray block;
  //QDataStream out(&block, QIODevice::WriteOnly);
  //out.setVersion(QDataStream::Qt_4_0);
  //    out << (quint16)6;
  //    out.device()->seek(0);
  //    out << (quint16)(block.size() - sizeof(quint16));

  tcpSocket.write(buffer.toStdString().c_str());
  tcpSocket.disconnectFromHost();
  tcpSocket.waitForDisconnected();
}

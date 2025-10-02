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

#include <QtNetwork>

#include <nqlogger.h>

#include "ThermoDAQServer.h"

ThermoDAQServer::ThermoDAQServer(ThermoDAQModel* model, QObject *parent)
    : QTcpServer(parent),
      model_(model)
{

}

void ThermoDAQServer::incomingConnection(int socketDescriptor)
{
    NQLog("ThermoDAQServer") << "incomingConnection(int socketDescriptor)";

    ThermoDAQServerThread *thread = new ThermoDAQServerThread(socketDescriptor, model_, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

ThermoDAQServerThread::ThermoDAQServerThread(int socketDescriptor,
                                             ThermoDAQModel* model, QObject *parent)
    : QThread(parent), socketDescriptor_(socketDescriptor), model_(model)
{

}

void ThermoDAQServerThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor_)) {
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

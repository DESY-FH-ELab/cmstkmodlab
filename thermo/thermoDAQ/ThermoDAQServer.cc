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

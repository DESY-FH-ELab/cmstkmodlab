#include <iostream>

#include <QtNetwork>

#include <nqlogger.h>

#include "MicroDAQServer.h"

MicroDAQServer::MicroDAQServer(MicroDAQModel* model, QObject *parent)
    : QTcpServer(parent),
      model_(model)
{

}

void MicroDAQServer::incomingConnection(int socketDescriptor)
{
    NQLog("MicroDAQServer") << "incomingConnection(int socketDescriptor)";

    MicroDAQServerThread *thread = new MicroDAQServerThread(socketDescriptor, model_, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

MicroDAQServerThread::MicroDAQServerThread(int socketDescriptor,
                                           MicroDAQModel* model, QObject *parent)
    : QThread(parent), socketDescriptor_(socketDescriptor), model_(model)
{

}

void MicroDAQServerThread::run()
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

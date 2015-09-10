#include <iostream>

#include <QtNetwork>

#include <nqlogger.h>

#include "DefoDAQServer.h"

DefoDAQServer::DefoDAQServer(DefoDAQModel* model, QObject *parent)
    : QTcpServer(parent),
      model_(model)
{

}

void DefoDAQServer::incomingConnection(int socketDescriptor)
{
    NQLog("DefoDAQServer") << "incomingConnection(int socketDescriptor)";

    DefoDAQServerThread *thread = new DefoDAQServerThread(socketDescriptor, model_, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

DefoDAQServerThread::DefoDAQServerThread(int socketDescriptor,
                                             DefoDAQModel* model, QObject *parent)
    : QThread(parent), socketDescriptor_(socketDescriptor), model_(model)
{

}

void DefoDAQServerThread::run()
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

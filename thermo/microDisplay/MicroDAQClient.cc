#include <iostream>

#include <QtNetwork>

#include <nqlogger.h>

#include "MicroDAQClient.h"

MicroDAQClient::MicroDAQClient(int socket, QObject *parent)
    :QObject(parent),
      socket_(socket)
{
    tcpSocket_ = new QTcpSocket(this);
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(read()));
}

void MicroDAQClient::readDAQStatus()
{
    NQLog("MicroDAQClient", NQLog::Spam) << "readDAQStatus()";

    blockSize_ = 0;
    tcpSocket_->connectToHost(QHostAddress::LocalHost, socket_);
    if (!tcpSocket_->waitForConnected(2000)) {
        tcpSocket_->abort();
        NQLog("MicroDAQClient", NQLog::Fatal) << "connection timeout";
    }
}

void MicroDAQClient::read()
{
    QByteArray in = tcpSocket_->readAll();
    QString buffer(in.data());

    tcpSocket_->abort();

    emit handleMessage(buffer);
}

#include <iostream>

#include <QtNetwork>

#include "ThermoDAQClient.h"

ThermoDAQClient::ThermoDAQClient(int socket, QObject *parent)
    :QObject(parent),
      socket_(socket)
{
    tcpSocket_ = new QTcpSocket(this);
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(read()));
}

void ThermoDAQClient::readDAQStatus()
{
    std::cout << "ThermoDAQClient::readDAQStatus()" << std::endl;

    blockSize_ = 0;
    tcpSocket_->abort();
    tcpSocket_->connectToHost(QHostAddress::LocalHost, socket_);
}

void ThermoDAQClient::read()
{
    std::cout << "ThermoDAQClient::read()" << std::endl;

    QByteArray in = tcpSocket_->readAll();
    QString buffer(in.data());

    emit handleMessage(buffer);
}

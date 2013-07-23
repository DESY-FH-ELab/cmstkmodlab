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
    tcpSocket_->connectToHost(QHostAddress::LocalHost, socket_);
    if (!tcpSocket_->waitForConnected(2000)) {
        tcpSocket_->abort();
        std::cout << "connection timeout" << std::endl;
    }
}

void ThermoDAQClient::read()
{
    // std::cout << "ThermoDAQClient::read()" << std::endl;

    QByteArray in = tcpSocket_->readAll();
    QString buffer(in.data());

    tcpSocket_->abort();

    emit handleMessage(buffer);
}

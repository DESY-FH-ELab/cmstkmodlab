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

//    QDataStream in(tcpSocket_);
//    in.setVersion(QDataStream::Qt_4_0);

//    std::cout << tcpSocket_->bytesAvailable() << std::endl;

//    if (blockSize_ == 0) {
//        if (tcpSocket_->bytesAvailable() < (int)sizeof(quint16))
//            return;

//        in >> blockSize_;
//    }

//    if (tcpSocket_->bytesAvailable() < blockSize_)
//        return;

//    QString nextFortune;
//    in >> nextFortune;

    std::cout << buffer.toStdString() << std::endl;
}

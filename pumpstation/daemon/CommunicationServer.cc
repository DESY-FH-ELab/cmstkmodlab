#include <iostream>

#include <QMutexLocker>

#include <nqlogger.h>
#include "ApplicationConfig.h"

#include "CommunicationServer.h"

CommunicationServer::CommunicationServer(ConradModel* conradModel,
                                         LeyboldGraphixThreeModel* leyboldModel,
                                         QObject *parent)
 : QTcpServer(parent),
   conradModel_(conradModel),
   leyboldModel_(leyboldModel)
{
  connect(this, SIGNAL(setSwitchEnabled(int, bool)),
          conradModel_, SLOT(setSwitchEnabled(int, bool)));
}

void CommunicationServer::incomingConnection(int socketDescriptor)
{
  NQLogDebug("server") << "void CommunicationServer::incomingConnection(int socketDescriptor)";

  socket_ = new QTcpSocket();

  connect(socket_, SIGNAL(readyRead()),
          this, SLOT(handleCommand()));
  connect(socket_, SIGNAL(disconnected()),
          socket_, SLOT(deleteLater()));

  if (!socket_->setSocketDescriptor(socketDescriptor)) {
    // emit error(tcpSocket->error());
    return;
  }
}

void CommunicationServer::handleCommand()
{
  NQLogDebug("server") << "void CommunicationServer::handleCommand()";

  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_4_0);

  quint16 blockSize = 0;
  in >> blockSize;

  QString command;
  in >> command;

  NQLogDebug("server") << "command: (" << blockSize << ") |" << command.toStdString() << "|";

  QStringList args = command.split(" ");
  QString cmd = args.at(0);
  args.removeAt(0);
  QString response;

  if (cmd=="setSwitchState") {
    if (args.count()!=2) {
      response = "ERR";
    } else {
      int channel = args.at(0).toInt();
      int state = args.at(1).toInt();

      if (channel<0 || channel>7) {
        response = "ERR";
      } else {
        emit setSwitchEnabled(channel, state);
        response = "OK";
      }
    }
  } else if (cmd=="getSwitchState") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int channel = args.at(0).toInt();

      if (channel<0 || channel>7) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        State state = conradModel_->getSwitchState(channel);
        response = QString::number((int)state);
      }
    }
  } else if (cmd=="getPressure") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int channel = args.at(0).toInt();

      if (channel<0 || channel>2) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        double pressure = leyboldModel_->getPressure(channel);
        response = QString("%1").arg(pressure, 0, 'f', 1);
      }
    }
  } else {
    response = "ERR";
  }

  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << (quint16)response.length();
  out << response;

  socket_->write(block);
  socket_->disconnectFromHost();
  socket_->waitForDisconnected();
}
